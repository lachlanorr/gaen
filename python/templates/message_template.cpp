//------------------------------------------------------------------------------
// <<message_name>>.h - Autogenerated message class for <<message_name>>
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2017 Lachlan Orr
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
namespace messages
{

template <typename T>
class <<message_name>>R
{
public:
    <<message_name>>R(const T & msgAcc)
      : mMsgAcc(msgAcc)
    {
<<reader_data_member_init>>
    }

<<reader_getters>>

private:
    <<message_name>>R(const <<message_name>>R &)              = delete;
    <<message_name>>R(const <<message_name>>R &&)             = delete;
    <<message_name>>R & operator=(const <<message_name>>R &)  = delete;
    <<message_name>>R & operator=(const <<message_name>>R &&) = delete;

    const T & mMsgAcc;

<<reader_data_members>>
};

typedef <<message_name>>R<MessageQueueAccessor> <<message_name>>QR;
typedef <<message_name>>R<MessageBlockAccessor> <<message_name>>BR;

class <<message_name>>QW : public MessageQueueWriter
{
public:
    <<message_name>>QW(u32 msgId,
    <<message_name_indent>> u32 flags,
    <<message_name_indent>> task_id source,
    <<message_name_indent>> task_id target<<payload_decl>>)
      : MessageQueueWriter(msgId,
                           flags,
                           source,
                           target,
                           to_cell(<<payload_value>>),
                           <<block_count>>)
    {}

    <<message_name>>QW(u32 msgId,
    <<message_name_indent>> u32 flags,
    <<message_name_indent>> task_id source,
    <<message_name_indent>> task_id target<<payload_decl>>,
    <<message_name_indent>> MessageQueue * pMsgQueue)
      : MessageQueueWriter(msgId,
                           flags,
                           source,
                           target,
                           to_cell(<<payload_value>>),
                           <<block_count>>,
                           pMsgQueue)
    {}

<<writer_setters>>
private:
    <<message_name>>QW(const <<message_name>>QW &)              = delete;
    <<message_name>>QW(const <<message_name>>QW &&)             = delete;
    <<message_name>>QW & operator=(const <<message_name>>QW &)  = delete;
    <<message_name>>QW & operator=(const <<message_name>>QW &&) = delete;
};

class <<message_name>>BW : public MessageBlockWriter
{
public:
    <<message_name>>BW(u32 msgId,
    <<message_name_indent>> u32 flags,
    <<message_name_indent>> task_id source,
    <<message_name_indent>> task_id target<<payload_decl>>)
      : MessageBlockWriter(msgId,
                           flags,
                           source,
                           target,
                           to_cell(<<payload_value>>),
                           <<block_count>>,
                           mBlocks)
    {}

<<writer_setters>>

private:
    <<message_name>>BW(const <<message_name>>BW &)              = delete;
    <<message_name>>BW(const <<message_name>>BW &&)             = delete;
    <<message_name>>BW & operator=(const <<message_name>>BW &)  = delete;
    <<message_name>>BW & operator=(const <<message_name>>BW &&) = delete;

    Block mBlocks[<<block_count>> + 1]; // +1 for header
};

} // namespace msg
} // namespace gaen

#endif // #ifndef GAEN_ENGINE_MESSAGES_<<message_name_caps>>MESSAGE_H

