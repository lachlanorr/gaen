//------------------------------------------------------------------------------
// main_osx.mm - OSX specific main function for gaen
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

#import <Cocoa/Cocoa.h>
#include <OpenGL/gl3.h>

#include "engine/renderer_api.h"
#include "renderergl/RendererGL.h"
#include "gaen/gaen.h"

//------------------------------------------------------------------------------

@interface OpenGLView : NSView
{
  @private
    NSOpenGLContext*     _openGLContext;
    NSOpenGLPixelFormat* _pixelFormat;
}
@end
@implementation OpenGLView
+ (NSOpenGLPixelFormat*)defaultPixelFormat
{
    NSOpenGLPixelFormatAttribute attrs[] =
        {
            NSOpenGLPFADoubleBuffer,
            0
        };
    NSOpenGLPixelFormat * pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
    return pixelFormat;
}

- (id)initWithFrame:(NSRect)frameRect
{
    return [self initWithFrame:frameRect pixelFormat:nil];
}

- (id)initWithFrame:(NSRect)frameRect pixelFormat:(NSOpenGLPixelFormat*)format
{
    self = [super initWithFrame:frameRect];
    if (self != nil)
    {
        if (format != nil)
            _pixelFormat = [format retain];
        else
            _pixelFormat = [OpenGLView defaultPixelFormat];
        
        _openGLContext = [[NSOpenGLContext alloc] initWithFormat:_pixelFormat shareContext:nil];
        [_openGLContext setView:self];
        
        [[NSNotificationCenter defaultCenter] addObserver:self
                                              selector:@selector(surfaceNeedsUpdate:)
                                              name:NSViewGlobalFrameDidChangeNotification
                                              object:self];
    }
    return self;    
}

- (void) surfaceNeedsUpdate:(NSNotification*)notification
{
   [self update];
}

- (void)lockFocus
{
    NSOpenGLContext* context = [self openGLContext];
 
    [super lockFocus];
    if ([context view] != self) {
        [context setView:self];
    }
    [context makeCurrentContext];
}

- (void)setOpenGLContext:(NSOpenGLContext*)context
{
    _openGLContext = context;
}

- (NSOpenGLContext*)openGLContext
{
    return _openGLContext;
}

- (void)clearGLContext
{
    [NSOpenGLContext clearCurrentContext];
}

- (void)prepareOpenGL
{
}

- (void)update
{
    [_openGLContext update];
}

- (void)setPixelFormat:(NSOpenGLPixelFormat*)pixelFormat
{
    _pixelFormat = pixelFormat;
}

- (NSOpenGLPixelFormat*)pixelFormat
{
    return _pixelFormat;
}

-(void) drawRect
{
    [_openGLContext makeCurrentContext];

    glClearColor(1.0f, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();

    [_openGLContext flushBuffer];
}

@end

//------------------------------------------------------------------------------

@interface AppDelegate : NSObject <NSApplicationDelegate>
@property (assign) int argc;
@property (assign) char ** argv;
@property (assign) id pool;
@property (assign) id window;
@property (assign) OpenGLView * view;
@end
@implementation AppDelegate
- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    using namespace gaen;
    static const u32 kScreenWidth = 1080;
    static const u32 kScreenHeight = 720;

    id menubar = [[NSMenu new] autorelease];
    id appMenuItem = [[NSMenuItem new] autorelease];
    [menubar addItem:appMenuItem];
    [NSApp setMainMenu:menubar];

    id appMenu = [[NSMenu new] autorelease];
    id appName = [[NSProcessInfo processInfo] processName];
    id quitTitle = [@"Quit " stringByAppendingString:appName];
    id quitMenuItem = [[[NSMenuItem alloc] initWithTitle:quitTitle
        action:@selector(terminate:) keyEquivalent:@"q"] autorelease];
    [appMenu addItem:quitMenuItem];
    [appMenuItem setSubmenu:appMenu];

    _window = [[[NSWindow alloc]
               initWithContentRect:NSMakeRect(0, 0, kScreenWidth, kScreenHeight)
               styleMask:NSTitledWindowMask backing:NSBackingStoreBuffered defer:NO]
              retain];

    _view = [[[OpenGLView alloc] initWithFrame:NSMakeRect(0, 0, kScreenWidth, kScreenHeight)] retain];
    [[_window contentView] addSubview:_view];

    [_window cascadeTopLeftFromPoint:NSMakePoint(20,20)];
    [_window setTitle:appName];

    [_window makeKeyAndOrderFront:nil];
    [_window setCollectionBehavior: NSWindowCollectionBehaviorFullScreenPrimary];
    

    init_gaen(_argc, _argv);

    RendererGL renderer;
    renderer.init(_view.openGLContext, nullptr, kScreenWidth, kScreenHeight);
    Task rendererTask = Task::create(&renderer, HASH::renderer);
    set_renderer(rendererTask);

    //[view drawRect];
    
    // NOTE: From this point forward, methods on sRenderer should not
    // be called directly.  Interaction with the renderer should only
    // be made with messages sent to the primary TaskMaster.

    start_game_loops();

}


- (void)applicationWillTerminate:(NSNotification *)aNotification
{
    gaen::fin_gaen();
}
@end

//------------------------------------------------------------------------------

int main(int argc, char ** argv)
{
    [NSAutoreleasePool new];

    [NSApplication sharedApplication];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

    AppDelegate * appDelegate = [[AppDelegate alloc] init];
    appDelegate.argc = argc;
    appDelegate.argv = argv;
    
    [NSApp setDelegate:(id)appDelegate];
    [NSApp activateIgnoringOtherApps:YES];
    [NSApp run];
    return 0;
}

