//------------------------------------------------------------------------------
// Messages.cpp - Messages window for composer application
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

#include "composer/stdafx.h"

#include "core/logging.h"

#include "composer/Messages.h"

namespace gaen
{

static bool key_accept(wchar_t)
{
    return false;
}

Messages::Messages(nana::size size)
  : BaseWindow(size)
{
    caption("Messages");

    //mText.editable(false);
    mText.select_behavior(false);
    mText.focus_behavior(nana::textbox::text_focus_behavior::none);
    mText.typeface(nana::paint::font("Consolas", 10));
    mText.set_accept(key_accept);

    mPlace.div("vert<menubar weight=28><text>");
    mPlace["menubar"] << mMenuBar;
    mPlace["text"] << mText;
    mPlace.collocate();


    mTimer.interval(100);
    mTimer.elapse([this]()
    {
        std::lock_guard<std::mutex> lock(mPendingMessagesMutex);
        for (LogMessage & lm : mPendingMessages)
        {
            std::stringstream ss;
            ss << lm.time << "\t" << lm.sev << "\t" << lm.msg << "\n";
            mText.append(ss.str(), false);
            mText.select(false);
        }
        mPendingMessages.clear();
    });
    mTimer.start();
}

Messages::~Messages()
{

}

void Messages::operator()(const char * time, const char * sev, const char * msg)
{
    std::lock_guard<std::mutex> lock(mPendingMessagesMutex);
    mPendingMessages.emplace_back(time, sev, msg);
}

} // namespace gaen
