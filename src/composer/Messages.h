//------------------------------------------------------------------------------
// MessagesWindow.h - MessagesWindow for composer application
//
// Gaen Concurrency Engine - http://gaen.org
// Copyright (c) 2014-2016 Lachlan Orr
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

#ifndef GAEN_COMPOSER_MESSAGES_WINDOW_H
#define GAEN_COMPOSER_MESSAGES_WINDOW_H

#include <memory>
#include <string>
#include <list>
#include <mutex>

#include "core/LogListener.h"

#include "composer/BaseWindow.h"

namespace gaen
{

class Messages : public BaseWindow
{
public:
    Messages(nana::size size);
    virtual ~Messages();

    void operator()(const char * time, const char * sev, const char * msg);

private:
    struct LogMessage
    {
        std::string time;
        std::string sev;
        std::string msg;

        LogMessage(const char * time, const char * sev, const char * msg)
          : time(time)
          , sev(sev)
          , msg(msg)
        {}
    };

    nana::textbox mText{*this};
    nana::timer mTimer;

    std::mutex mPendingMessagesMutex;
    std::list<LogMessage> mPendingMessages;

    LogListener<Messages> mLogListener{*this};
}; // class MessagesWindow


} // namespace gaen


#endif // #ifndef GAEN_COMPOSER_MESSAGES_WINDOW_H
