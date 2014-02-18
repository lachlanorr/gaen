//------------------------------------------------------------------------------
// main_win32.cpp - Gaen game app
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

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include "core/logging.h"
#include "renderergl/gaen_opengl.h"

namespace gaen
{

static const char * kClassName = "GaenMainWindow";

static HINSTANCE sHinstance = 0;
static HWND sHwnd = 0;
static HDC sHdc = 0;
static HGLRC sHrc = 0;

static bool sFullScreen = false;

static const size_t kKeyCount = 256;
static bool sKeys[kKeyCount];

LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {

        // TODO: LORR 2011-06-01 - Consider handling WM_ACTIVATE to pause on minimize
        //case WM_ACTIVATE:
        //active_ = !HIWORD(wParam);
        //return 0;

    case WM_SYSCOMMAND:
        // Prevent screen saver and power save modes
        switch(wParam)
        {
        case SC_SCREENSAVE:
        case SC_MONITORPOWER:
            return 0;
        }
        break;

    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;

    case WM_KEYDOWN:
        sKeys[wParam] = true;
        return 0;

    case WM_KEYUP:
        sKeys[wParam] = false;
        return 0;

    case WM_LBUTTONDOWN:
        //pmPrimary_->eventMgr().queueEvent(new MouseButton(LOWORD(lParam), HIWORD(lParam), eMouseButton_Left, eMouseButtonState_Down));
        return 0;

    case WM_LBUTTONUP:
        //pmPrimary_->eventMgr().queueEvent(new MouseButton(LOWORD(lParam), HIWORD(lParam), eMouseButton_Left, eMouseButtonState_Up));
        return 0;

    case WM_RBUTTONDOWN:
        //pmPrimary_->eventMgr().queueEvent(new MouseButton(LOWORD(lParam), HIWORD(lParam), eMouseButton_Right, eMouseButtonState_Down));
        return 0;

    case WM_RBUTTONUP:
        //pmPrimary_->eventMgr().queueEvent(new MouseButton(LOWORD(lParam), HIWORD(lParam), eMouseButton_Right, eMouseButtonState_Up));
        return 0;

    case WM_MBUTTONDOWN:
        //pmPrimary_->eventMgr().queueEvent(new MouseButton(LOWORD(lParam), HIWORD(lParam), eMouseButton_Middle, eMouseButtonState_Down));
        return 0;

    case WM_MBUTTONUP:
        //pmPrimary_->eventMgr().queueEvent(new MouseButton(LOWORD(lParam), HIWORD(lParam), eMouseButton_Middle, eMouseButtonState_Up));
        return 0;

    case WM_MOUSEMOVE:
        //pmPrimary_->eventMgr().queueEvent(new MouseMove(LOWORD(lParam), HIWORD(lParam)));
        return 0;


    case WM_SIZE:
        // LoWord=Width, HiWord=Height
        //RendererS::inst().resizeScene(LOWORD(lParam),HIWORD(lParam)); 
        return 0;
    }

    return DefWindowProcW(hWnd,msg,wParam,lParam);
}

void killGLWindow()
{
    if (sFullScreen)
    {
        ChangeDisplaySettings(NULL, 0); // switch back to windowed
        ShowCursor(TRUE);
    }

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

    if (!UnregisterClassA(kClassName, sHinstance))
        PANIC("Failed to unregister window class");

}

void createGLWindow(const char * title, u32 screenWidth, u32 screenHeight, u32 bitsPerPixel)
{
    GLuint pixelFormat;
    WNDCLASSA wc;
    DWORD exStyle;
    DWORD style;


    RECT windowRect;
    windowRect.left = 0;
    windowRect.right = screenWidth;
    windowRect.top = 0;
    windowRect.bottom = screenHeight;

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
    if (!RegisterClassA(&wc))
        PANIC("Failed to register WNDCLASS");
        

    if (sFullScreen)
    {
        DEVMODEA screenSettings;
        memset(&screenSettings, 0, sizeof(screenSettings));
        screenSettings.dmSize = sizeof(screenSettings);
        screenSettings.dmPelsWidth = screenWidth;
        screenSettings.dmPelsHeight = screenHeight;
        screenSettings.dmBitsPerPel = bitsPerPixel;
        screenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        // Try to set full screen mode
        if (ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
        {
            LOG_WARNING(NULL, "The requested fullscreen mode is not supported.  Defaulting to windowed mode.",
                        "Graphics Error", MB_OK | MB_ICONEXCLAMATION);
            sFullScreen = false;
        }
    }

    // Still in full screen?
    if (sFullScreen)                
    {
        exStyle = WS_EX_APPWINDOW;
        style = WS_POPUP;
        ShowCursor(FALSE);
    }
    else
    {
        exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
        style = WS_OVERLAPPEDWINDOW;
    }

    AdjustWindowRectEx(&windowRect, style, FALSE, exStyle);

    // Create The Window
    sHwnd = CreateWindowExA(exStyle,
                            kClassName,
                            title,
                            style | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                            0, 0,
                            windowRect.right - windowRect.left,
                            windowRect.bottom - windowRect.top,
                            NULL,
                            NULL,
                            sHinstance,
                            NULL);
    if (!sHwnd)
    {
        killGLWindow();
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

    if(!wglMakeCurrent(sHdc, sHrc))
        PANIC("Cannot activate GL rendering context");


    // Prepare our GL function pointers.
    // We have to wait until here to do this since if you call it too
    // early, the GL driver dll hasn't been loaded and
    // wglGetProcAddress will return NULL for all functions.
    init_win32gl();

    // Show and make our window have focus
    ShowWindow(sHwnd, SW_SHOW);
    SetForegroundWindow(sHwnd);
    SetFocus(sHwnd);

    // Set Up Our Perspective GL Screen
    // TODO: LORR 2011-06-28 - resizeScene gets called twice at startup.  Once here and once during WM_SIZE handler.
    // This extra call may only be necessary in full screen mode, or may not be necessary at all.
    //RendererS::inst().resizeScene(screenWidth, screenHeight);

    // Initialize newly created GL window
    //RendererS::inst().initGL();

}

} // namespace gaen

int CALLBACK WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow)
{
    gaen::sHinstance = hInstance;

    return 0;
}
