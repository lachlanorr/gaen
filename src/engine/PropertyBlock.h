//------------------------------------------------------------------------------
// PropertyBlock.h - Base class for property blocks
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2015 Lachlan Orr
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//   1. The origin of this software must not be misrepresented; you must not
//   claim that you wrote the original software. If you use this software
//   in a product, an acknowledgment in the product documentation would be
//   appreciated but is not required.
//
//   2. Altered source versions must be plainly marked as such, and must not be
//   misrepresented as being the original software.
//
//   3. This notice may not be removed or altered from any source
//   distribution.
//------------------------------------------------------------------------------

#ifndef GAEN_ENGINE_PROPERTYBLOCK_H
#define GAEN_ENGINE_PROPERTYBLOCK_H

#include "engine/MessageWriter.h"

namespace gaen
{

class PropertyBlock
{
public:
    PropertyBlock()
    {
        // Initialize properties.
        setMessage(nullptr);
        
        // Ensure unused parts of mBlocks are zero'd out.
        mBlocks[0].d[1] = 0;
    }

    
    template <typename T>
    MessageResult message(const Message& msg, const T msgAcc)
    {
        u32 property_id;
        u32 version;
        
        switch (msg.msgId)
        {

        case HASH::get_property:
            // Get the value of a single property, which will be sent
            // as a "property" message back to source task.
            property_id = msg.payload.u;

            switch (property_id)
            {
            case HASH::model:
                MessageWriter msgwr(HASH::property,
                                    kMessageFlag_None,
                                    msg.target,
                                    msg.source,
                                    property_id,
                                    1);
                msgwr.accessor()[0].d[0].p = model();
                return MessageResult::Consumed;
            }

            ERR("Unexpected get_property for PropertyBlock, property_id = %d", property_id);
            return MessageResult::Propogate;

        case HASH::set_property:
            // Set the value fo a single property. No response will
            // be sent.
            property_id = msg.payload.u;

            switch (property_id)
            {
            case HASH::model:
                setModel(reinterpret_cast<Model *>(msgAcc[0].d[0].p));
                return MessageResult::Consumed;
            }

            ERR("Unexpected set_property for PropertyBlock, property_id = %d", property_id);
            return MessageResult::Propogate;

        case HASH::get_all_properties:
            // Retrieve all properties, in as many blocks as
            // necessary.  An "all_properties" message containing all
            // property data back to source task.
            MessageWriter msgwr(HASH::all_properties,
                                kMessageFlag_None,
                                msg.target,
                                msg.source,
                                kVersion,
                                kBlockCount);
            for (u32 i = 0; i < kBlockCount ++i)
            {
                msgwr.accessor()[i] = mBlocks[i];
            }
            return MessageResult::Consumed;

        case HASH::set_all_properties:
            // Set all properties.  If versions don't match, a
            // "property_version_mismatch" message will be sent back
            // to source task, and no changes to properties will be
            // made.
            version = msg.payload.u;

            if (version == kVersion)
            {
                for (u32 i = 0; i < kBlockCount ++i)
                {
                    mBlocks[i] = msgwr.accessor()[i];
                }
            }
            else
            {
                // versions don't match, return error response
                MessageWriter msgwr(HASH::property_version_mismatch,
                                    kMessageFlag_None,
                                    msg.target,
                                    msg.source,
                                    kVersion,
                                    kBlockCount);
            }
            return MessageResult::Consumed;
        }

        ERR("Unexpected message sent to PropertyBlock, msgId = %d", msg.msgId);
        return MessageResult::Propogate;

    }

    // Simple accessors
    Model * model() {}
    void setModel(const Model * pModel) {}

private:
    static const fnv kVersion = 0xDEADbeef; // fnv hash of this version of the properties

    static const u32 kBlockCount = 1;
    PropertyBlock mBlocks[kBlockCount];
};

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_PROPERTYBLOCK_H
