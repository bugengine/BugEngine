/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include    <bugengine/plugin.graphics.windowing/stdafx.h>
#include    <bugengine/plugin.graphics.windowing/renderer.hh>
#include    <bugengine/plugin.graphics.windowing/window.hh>
#include    <bugengine/core/threads/event.hh>

#include    <darwin/platformrenderer.hh>

#ifndef MAC_OS_X_VERSION_10_12
# define MAC_OS_X_VERSION_10_12 101200
#endif

#if __MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_12
# define NSEventMaskAny NSAnyEventMask
#endif

namespace BugEngine { namespace Windowing
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
    while(NSEvent* event = [m_application nextEventMatchingMask:NSEventMaskAny
                                                      untilDate:nil
                                                         inMode:NSDefaultRunLoopMode
                                                        dequeue:YES])
    {
        [m_application sendEvent:event];
        [m_application updateWindows];
    }
}

Renderer::Renderer(minitl::Allocator& arena, weak<Resource::ResourceManager> manager)
    :   IRenderer(arena, manager, Scheduler::MainThread)
    ,   m_platformRenderer(scoped<PlatformRenderer>::create(arena))
{
}

Renderer::~Renderer()
{
}

uint2 Renderer::getScreenSize() const
{
    NSArray* screens = [NSScreen screens];
    NSScreen* screen = [screens objectAtIndex:0];
    NSRect frame = [screen visibleFrame];
    return make_uint2(frame.size.width, frame.size.height);
}

void Renderer::flush()
{
    IRenderer::flush();
    m_platformRenderer->flush();
}

bool Renderer::success() const
{
    return true;
}

}}
