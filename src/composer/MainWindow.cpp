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

#include "composer/MainWindow.h"

namespace gaen
{
void on_menu_item(nana::menu::item_proxy& ip)
{
    return;
}

MainWindow::MainWindow()
  : nana::form(nana::rectangle(nana::size(1280, 720)))
{
    mpMenu.reset(new nana::menu());
    mpMenu->append("item1", on_menu_item);
    (*this)["menu"] << *mpMenu;


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

    mpSubForm.reset(new nana::nested_form(*this));
    //Define a label and display a text.
    mpLabel2.reset(new nana::label(*mpSubForm, "Hello2, <bold blue size=16>Nana C++ Library</>"));
    mpLabel2->format(true);
    mpSubForm->div("vert <><<><weight=80% text><>>");
    (*mpSubForm)["text"] << *mpLabel2;
    mpSubForm->collocate();
    mpSubForm->show();
}

MainWindow::~MainWindow()
{

}

} // namespace
