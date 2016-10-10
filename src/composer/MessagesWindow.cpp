//------------------------------------------------------------------------------
// MessagesWindow.cpp - MessagesWindow for composer application
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

#include "composer/stdafx.h"

#include "composer/MessagesWindow.h"

namespace gaen
{

MessagesWindow::MessagesWindow(nana::form & form)
  : nana::nested_form(form)
{
    mpLabel2.reset(new nana::label(*this, "Hello2, <bold blue size=16>Nana C++ Library</>"));
    mpLabel2->format(true);
    this->div("vert <><<><weight=80% text><>>");
    (*this)["text"] << *mpLabel2;
}

MessagesWindow::~MessagesWindow()
{

}


} // namespace gaen
