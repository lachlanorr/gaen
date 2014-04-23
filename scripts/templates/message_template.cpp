//------------------------------------------------------------------------------
// <<message_name>>Message.h - Autogenerated message class for <<message_name>>
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

#ifndef GAEN_ENGINE_MESSAGES_<<message_name_caps>>MESSAGE_H
#define GAEN_ENGINE_MESSAGES_<<message_name_caps>>MESSAGE_H

<<includes>>
namespace gaen
{

class <<message_name>>Reader
{
public:
    <<message_name>>Reader(const MessageQueue::MessageAccessor & msgAcc)
      : mMsgAcc(msgAcc)
    {
<<reader_data_member_init>>
    }

<<reader_getters>>
        
private:
    const MessageQueue::MessageAccessor & mMsgAcc;

<<reader_data_members>>    
};



class <<message_name>>Writer : protected MessageWriter
{
public:
    <<message_name>>Writer(fnv msgId,
    <<message_name_indent>>       u32 flags,
    <<message_name_indent>>       task_id source,
    <<message_name_indent>>       task_id target<<payload_decl>>)
      : MessageWriter(msgId,
                      flags,
                      source,
                      target,
                      to_cell(<<payload_value>>),
                      <<block_count>>) {}
    
<<writer_setters>>
};

} // namespace gaen

#endif // #ifndef GAEN_ENGINE_MESSAGES_<<message_name_caps>>MESSAGE_H

