/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>

#include    <main/main.hh>

/*---------------------------------------------------------------------------*/
int be_main (BugEngine::Application* app)
{
    BugEngine::Graphics::WindowFlags f;
    f.position = BugEngine::int2(0,0);
    f.size = BugEngine::uint2(1280,800);
    f.title = "BugEngine editor v0.0";
    f.border = true;
    f.fullscreen = false;
    f.vsync = false;
    f.triplebuffered = false;

    return app->run();
}
/*---------------------------------------------------------------------------*/
