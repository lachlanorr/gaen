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

#include "gaen/gaen.h"
#include "engine/hashes.h"


@interface AppDelegate : NSObject <NSApplicationDelegate>
@property (assign) id pool;
@property (assign) id window;
@end

@implementation AppDelegate
- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
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
               initWithContentRect:NSMakeRect(0, 0, 1080, 720)
               styleMask:NSTitledWindowMask backing:NSBackingStoreBuffered defer:NO]
              retain];
    [_window cascadeTopLeftFromPoint:NSMakePoint(20,20)];
    [_window setTitle:appName];
    [_window makeKeyAndOrderFront:nil];
    [_window setCollectionBehavior: NSWindowCollectionBehaviorFullScreenPrimary];
    [_window makeMainWindow];
}


- (void)applicationWillTerminate:(NSNotification *)aNotification
{

}
@end


int main(int argc, char ** argv)
{
    [NSAutoreleasePool new];

    [NSApplication sharedApplication];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

    id appDelegate = [[AppDelegate alloc] init];
    [NSApp setDelegate:appDelegate];
    [NSApp activateIgnoringOtherApps:YES];
    [NSApp run];
    return 0;

    // Initialize gaen
//    gaen::init_gaen(argc, argv);
//    gaen::fnv h = gaen::HASH::fnv1a_32("abc");

    // Finalize gaen
//    gaen::fin_gaen();
}

