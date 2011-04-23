/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
 see LICENSE for detail */

#include    <windowing/stdafx.h>
#include    <windowing/renderer.hh>
#include    <windowing/window.hh>
#include    <core/threads/event.hh>

#include    <darwin/platformrenderer.hh>


namespace BugEngine { namespace Graphics { namespace Windowing
{

Renderer::PlatformRenderer::PlatformRenderer()
{
    m_pool = [[NSAutoreleasePool alloc] init];
    m_application = [NSApplication sharedApplication];

    NSMenu* menu = [[NSMenu alloc] initWithTitle:@"bugengine"];
    NSMenuItem* mi = [menu addItemWithTitle:@"Apple" action:Nil keyEquivalent:@""];
    NSMenu* m = [[NSMenu alloc] initWithTitle:@"Apple"];
    [menu setSubmenu: m forItem: mi];
    mi = [m addItemWithTitle:@"Quit" action:Nil keyEquivalent:@""];
    [mi setTarget:m_application];
    [mi setAction:@selector(stop:)];
    [NSApp performSelector:@selector(setAppleMenu:) withObject: m];
    [m_application setMainMenu: menu];
    [m_application finishLaunching];
}

Renderer::PlatformRenderer::~PlatformRenderer()
{
    [m_pool release];
}

void Renderer::PlatformRenderer::flush()
{
    [m_pool release];
    m_pool = [[NSAutoreleasePool alloc] init];
    while(NSEvent* event = [m_application nextEventMatchingMask:NSAnyEventMask
                                                      untilDate:nil
                                                         inMode:NSDefaultRunLoopMode
                                                        dequeue:YES])
    {
        [m_application sendEvent:event];
        [m_application updateWindows];
    }
}

Renderer::Renderer()
    :   m_platformRenderer(scoped<PlatformRenderer>::create(gameArena()))
{
}

Renderer::~Renderer()
{
}

uint2 Renderer::getScreenSize()
{
    NSArray* screens = [NSScreen screens];
    NSScreen* screen = [screens objectAtIndex:0];
    NSRect frame = [screen visibleFrame];
    return uint2(frame.size.width, frame.size.height);
}

void Renderer::flush()
{
    m_platformRenderer->flush();
}

void Renderer::createContextAsync(void* params)
{
    createContext(params);
}

void Renderer::destroyContextAsync()
{
    destroyContext();
}

}}}
