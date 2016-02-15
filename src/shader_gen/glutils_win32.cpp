//------------------------------------------------------------------------------
// glutils_win32.cpp - Windows specific opengl code
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

#include <windows.h>

#include "core/base_defines.h"

#if IS_PLATFORM_WIN32
#include "renderergl/win32gl.h"
#endif

namespace gaen
{

static const char * kClassName = "ShaderGenWindow";
static HINSTANCE sHinstance = 0;
static HWND sHwnd = 0;
static HDC sHdc = 0;
static HGLRC sHrc = 0;

LRESULT CALLBACK wndProc(HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}

void init_opengl()
{
    GLuint pixelFormat;
    WNDCLASS wc;
    DWORD exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    DWORD style = WS_OVERLAPPEDWINDOW| WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

    wc.style                = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc          = (WNDPROC)wndProc;
    wc.cbClsExtra           = 0;
    wc.cbWndExtra           = 0;
    wc.hInstance            = sHinstance;
    wc.hIcon                = LoadIcon(NULL, IDI_WINLOGO);
    wc.hCursor              = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground        = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName         = NULL;
    wc.lpszClassName        = kClassName;

    // Register windows class
    if (!RegisterClass(&wc))
        PANIC("Failed to register WNDCLASS");
        
    // Create The Window
    sHwnd = CreateWindowEx(exStyle,
                           kClassName,
                           "ShaderGenTitle",
                           style | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                           0, 0,
                           200,
                           200,
                           NULL,
                           NULL,
                           sHinstance,
                           NULL);
    if (!sHwnd)
    {
        PANIC("Window Creation Error");
    }


    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    
    // Set what's important, all else left 0
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

        
    if (!(sHdc = GetDC(sHwnd)))
        PANIC("Cannnot create GL device context");

    if (!(pixelFormat = ChoosePixelFormat(sHdc, &pfd)))
        PANIC("Cannot find suitable pixel format");

    if(!SetPixelFormat(sHdc, pixelFormat, &pfd))
        PANIC("Cannot set pixel format");

    if (!(sHrc = wglCreateContext(sHdc)))
        PANIC("Cannot create GL rendering context");


   if (!wglMakeCurrent(sHdc, sHrc))
        PANIC("Cannot activate GL rendering context");

    // Prepare our GL function pointers.
    // We have to wait until here to do this since if you call it too
    // early, the GL driver dll hasn't been loaded and
    // wglGetProcAddress will return NULL for all functions.
    init_win32gl();
}

void fin_opengl()
{
    if (sHrc)
    {
        if (!wglMakeCurrent(NULL, NULL))
            PANIC("Failed to make our rendering context current");

        if (!wglDeleteContext(sHrc))
            PANIC("Failed to release rendering context");
    }

    if (sHdc && !ReleaseDC(sHwnd, sHdc))
        PANIC("Failed to release device context");

    if (sHwnd && !DestroyWindow(sHwnd))
        PANIC("Failed to destroy window");

    if (!UnregisterClass(kClassName, sHinstance))
        PANIC("Failed to unregister window class");

}

}


