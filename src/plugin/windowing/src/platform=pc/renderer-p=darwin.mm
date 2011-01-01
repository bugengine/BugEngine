/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
 see LICENSE for detail */

#include    <windowing/stdafx.h>
#include    <windowing/renderer.hh>
#include    <windowing/window.hh>
#include    <core/threads/event.hh>

#include    <Cocoa/Cocoa.h>


namespace BugEngine { namespace Graphics { namespace Windowing
{

class Renderer::PlatformRenderer : public minitl::refcountable
{
private:
    NSAutoreleasePool*  m_pool;
    NSApplication*      m_application;
public:
    PlatformRenderer();
    ~PlatformRenderer();
    void flush();
};

Renderer::PlatformRenderer::PlatformRenderer()
{
    NSAutoreleasePool* m_pool = [[NSAutoreleasePool alloc] init];
    m_application = [NSApplication sharedApplication];

    NSWindow *window;
    NSView *view;
    
    view = [[NSView alloc] initWithFrame:NSMakeRect(0,100,200,200)];
    window = [[NSWindow alloc] initWithContentRect:NSMakeRect(50,100,200,300) 
                                         styleMask:NSTitledWindowMask | NSResizableWindowMask
                                           backing:NSBackingStoreBuffered
                                             defer:TRUE];
    NSTextField *text=[[NSTextField alloc]initWithFrame:NSMakeRect(10,60,180,22)];
    [text setStringValue:@"sample text"];
    NSButton *button=[[NSButton alloc]initWithFrame:NSMakeRect(10,10,180,32)];
    [button setBezelStyle:NSRoundedBezelStyle];
    [button setTitle:@"Quit"];
    [button setTarget:m_application];
    [button setAction:@selector(stop:)];
    [window setTitle:@"test1"];
    [[window contentView] addSubview:view];
    [[window contentView] addSubview:text];
    [[window contentView] addSubview:button];

    NSMenu* menu = [[NSMenu alloc] initWithTitle:@"bugengine"];
    NSMenuItem* mi = [menu addItemWithTitle:@"Apple" action:Nil keyEquivalent:@""];
    NSMenu* m = [[NSMenu alloc] initWithTitle:@"Apple"];
    [menu setSubmenu: m forItem: mi];
    mi = [m addItemWithTitle:@"Quit" action:Nil keyEquivalent:@""];
    [mi setTarget:m_application];
    [mi setAction:@selector(stop:)];
    [NSApp performSelector:@selector(setAppleMenu:) withObject: m];
    [m_application setMainMenu: menu];
    [window makeKeyAndOrderFront: nil];

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
    return uint2(1920, 1200);
}

void Renderer::flush()
{
    m_platformRenderer->flush();
}

void Renderer::createContextAsync(void*)
{
}

void Renderer::destroyContextAsync(void* params)
{
}

void Renderer::createContext(void*)
{
}

void Renderer::destroyContext(void*)
{
}

}}}
