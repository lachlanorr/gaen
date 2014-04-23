//------------------------------------------------------------------------------
// ModelComponent.h - Component that encapsulate a static model
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014 Lachlan Orr
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

#ifndef GAEN_ENGINE_MODEL_COMPONENT_H
#define GAEN_ENGINE_MODEL_COMPONENT_H

#include "engine/Model.h"
#include "engine/Component.h"

namespace gaen
{

// Base component that handles some generic stuff all components must
// abide by

class ModelComponent : public Component
{
public:
    ModelComponent(Model * pModel)
      : mpModel(pModel) {}
    
    void update(f32 deltaSecs);
    MessageResult message(const MessageQueue::MessageAccessor& msgAcc);

private:
    //--------------------------------------------------------------------------
    // Properties
    //--------------------------------------------------------------------------
    class Properties
    {
    public:
        static const fnv kVersion = 0xDEADbeef; // fnv hash of this version of the properties

        Properties()
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

            PropertyGroup group = PropertyGroup::Current;
            PropertyBlock * pBlock;
        
            switch (msg.msgId)
            {

            // get_property
            case FNV::get_property_initial:
                group = PropertyGroup::Initial;
            case FNV::get_property:
                // Get the value of a single property, which will be sent
                // as a "property" message back to source task.
                property_id = msg.payload.u;

                switch (property_id)
                {
                case FNV::model:
                    MessageWriter msgwr(group == PropertyGroup::Current ? FNV::property : FNV::property_initial,
                                        kMessageFlag_None,
                                        msg.target,
                                        msg.source,
                                        property_id,
                                        1);
                    msgwr.accessor()[0].d[0].p = model(group);
                    return MessageResult::Consumed;
                }

                ERR("Unexpected get_property for ModelComponent, property_id = %d", property_id);
                return MessageResult::Propogate;

            // set_property
            case FNV::set_property_initial:
                group = PropertyGroup::Initial;
            case FNV::set_property:
                // Set the value fo a single property. No response will
                // be sent.
                property_id = msg.payload.u;

                switch (property_id)
                {
                case FNV::model:
                    setModel(reinterpret_cast<Model *>(msgAcc[0].d[0].p), group);
                    return MessageResult::Consumed;
                }

                ERR("Unexpected set_property for ModelComponent, property_id = %d", property_id);
                return MessageResult::Propogate;

            // reset_property
            case FNV::reset_property:
                property_id = msg.payload.u;

                switch (property_id)
                {
                case FNV::model:
                    resetModel();
                    return MessageResult::Consumed;
                }

                ERR("Unexpected reset_property for ModelComponent, property_id = %d", property_id);
                return MessageResult::Propogate;

            // reset_properties
            case FNV::reset_properties:
                resetProperties();
                return MessageResult::Consumed;
            }

            // get_properties
            case FNV::get_properties_initial:
                group = PropertyGroup::Initial;
            case FNV::get_properties:
                // Retrieve all properties, in as many blocks as
                // necessary.  An "current_properties" message containing all
                // property data back to source task.
                pBlock = block(group);
                MessageWriter msgwr(group == PropertyGroup::Current ? FNV::properties : FNV::properties_initial,
                                    kMessageFlag_None,
                                    msg.target,
                                    msg.source,
                                    kVersion,
                                    kBlockCount);
                for (u32 i = 0; i < kBlockCount ++i)
                {
                    msgwr.accessor()[i] = pBlock[i];
                }
                return MessageResult::Consumed;

            // set_properties
            case FNV::set_properties_initial:
                group = PropertyGroup::Initial;
            case FNV::set_properties:
                // Set all properties.  If versions don't match, a
                // "property_version_mismatch" message will be sent back
                // to source task, and no changes to properties will be
                // made.
                pBlock = block(group);
                version = msg.payload.u;

                if (version == kVersion)
                {
                    for (u32 i = 0; i < kBlockCount ++i)
                    {
                        pBlock[i] = msgwr.accessor()[i];
                    }
                }
                else
                {
                    // versions don't match, return error response
                    MessageWriter msgwr(FNV::property_version_mismatch,
                                        kMessageFlag_None,
                                        msg.target,
                                        msg.source,
                                        kVersion,
                                        kBlockCount);
                }
                return MessageResult::Consumed;


            return MessageResult::Propogate;
        }


        // Accessors
        Model * model(PropertyGroup group = PropertyGroup::Current)
        {
            return block(group)[0].d[0].p;
        }
        
        void setModel(const Model * pModel, PropertyGroup group = PropertyGroup::Current)
        {
            block(group)[0].d[0].p = pModel;
        }

        void resetModel()
        {
            mCurrentValues[0].d[0].p = mInitialValues[0].d[0].p;
        }


        // Utilities
        bool havePropertiesChanged()
        {
            for (u32 i = 0; i < kBlockCount; ++i)
            {
                if (mCurrentValues[i] != mInitialValues[i])
                    return true;
            }
            return false;
        }

        bool restProperties()
        {
            for (u32 i = 0; i < kBlockCount; ++i)
            {
                mCurrentValues[i] = mInitialValues[i];
            }
        }
    private:
        PropertyBlock * block(PropertyGroup group)
        {
            return group == PropertyGroup::Current ? mCurrentvalues : mInitialValues;
        }
        
        static const u32 kBlockCount = 1;
        PropertyBlock mCurrentValues[kBlockCount];
        PropertyBlock mInitialValues[kBlockCount];
    }; // class Properties
    //--------------------------------------------------------------------------
    // Properties (END)
    //--------------------------------------------------------------------------

    Properties mProperties;

    static_assert(sizeof(ModelComponent) == sizeof(ModelComponent::Properties, "ModelComponent cannot contain data members outside of Properties")

};


} // namespace gaen

#endif // #ifndef GAEN_ENGINE_MODEL_COMPONENT_H
