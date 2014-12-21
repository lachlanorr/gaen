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

@interface GLView : NSOpenGLView
@end
@implementation GLView
- (void) drawRect: (NSRect) bounds
{
//    glClearColor(0, 0, 0, 0);
//    glClear(GL_COLOR_BUFFER_BIT);
//    glFlush();
}
@end

//------------------------------------------------------------------------------

@interface AppDelegate : NSObject <NSApplicationDelegate>
@property (assign) int argc;
@property (assign) char ** argv;
@property (assign) id pool;
@property (assign) id window;
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

    GLView *view = [[GLView alloc] initWithFrame:NSMakeRect(0, 0, kScreenWidth, kScreenHeight)];
    [[_window contentView] addSubview:view];

    [_window cascadeTopLeftFromPoint:NSMakePoint(20,20)];
    [_window setTitle:appName];

    [_window makeKeyAndOrderFront:nil];
    [_window setCollectionBehavior: NSWindowCollectionBehaviorFullScreenPrimary];
    

    init_gaen(_argc, _argv);

    RendererGL renderer;
    renderer.init(0, 0, kScreenWidth, kScreenHeight);
    Task rendererTask = Task::create(&renderer, HASH::renderer);
    set_renderer(rendererTask);

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

