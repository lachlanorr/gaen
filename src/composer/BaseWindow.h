//------------------------------------------------------------------------------
// BaseWindow.h - BaseWindow for composer application
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

#ifndef GAEN_COMPOSER_BASE_WINDOW_H
#define GAEN_COMPOSER_BASE_WINDOW_H

#include <memory>

namespace nana
{
class form;
class menubar;
class label;
class button;
}

namespace gaen
{

class BaseWindow : public nana::form
{

public:
    BaseWindow(nana::size size);
    virtual ~BaseWindow();

private:
    void buildMenus();

    void dockable__();

    std::unique_ptr<nana::place> mpPlace;
    std::unique_ptr<nana::menubar> mpMenuBar;
    std::unique_ptr<nana::label> mpLabel;
    std::unique_ptr<nana::button> mpButton;

}; // class BaseWindow

} // namespace gaen

#endif // #ifndef GAEN_COMPOSER_BASE_WINDOW_H
