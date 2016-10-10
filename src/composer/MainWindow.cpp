//------------------------------------------------------------------------------
// MainWindow.cpp - MainWindow for composer application
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

#include "core/logging.h"

#include "composer/MessagesWindow.h"

#include "composer/MainWindow.h"

namespace gaen
{
MainWindow::MainWindow()
  : nana::form(nana::rectangle(nana::size(1280, 720)))
{

    //Define a label and display a text.
    mpLabel.reset(new nana::label(*this, "Hello, <bold blue size=16>Nana C++ Library</>"));
    mpLabel->format(true);

    //Define a button and answer the click event.
    mpButton.reset(new nana::button(*this, "Quit"));
    mpButton->events().click([this]{
        close();
    });

    //Layout management
    div("vert <><<><weight=80% text><>><><weight=24<><button><>><>");
    (*this)["text"] << *mpLabel;
    (*this)["button"] << *mpButton;
    collocate();

    mpMessagesWindow.reset(new MessagesWindow(*this));
    mpMessagesWindow->collocate();
    mpMessagesWindow->show();
}

MainWindow::~MainWindow()
{

}



void on_menu_item(nana::menu::item_proxy& ip)
{
    LOG_INFO("on_menu_item clicked");
}

void MainWindow::buildMenus()
{
    mpMenuBar.reset(new nana::menubar(this->handle()));
    nana::menu & m = mpMenuBar->push_back("View");
    m.append("Messages", on_menu_item);

    //mpMessagesWindow->
}


} // namespace
