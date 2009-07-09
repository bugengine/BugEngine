/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU General Public License as published by the Free  *
* Software Foundation; either version 2 of the License, or (at your option)   *
* any later version.                                                          *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for   *
* more details.                                                               *
*                                                                             *
* You should have received a copy of the GNU General Public License along     *
* with this program; if not, write to                                         *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

#include    <stdafx.h>

#include    <main/main.hh>

#include    <mainscene.hh>

#include    <core/threads/thread.hh>

#include    <rtti/test.hh>
#include    <rtti/namespace.hh>

/*---------------------------------------------------------------------------*/
int be_main (BugEngine::Application* app)
{
    TestNS::Test* t = new TestNS::Test;
    BugEngine::RTTI::Namespace::root()->insert("Sub1.Sub2.test", refptr<BugEngine::Object>(t));
    BugEngine::Plugin p("lua");
    void (*doFile)(const char *file) = p.get<void(*)(const char *)>("doFile");
    (*doFile)("data/scripts/main.lua");

    BugEngine::Graphics::WindowFlags f;
    f.position = BugEngine::int2(0,0);
    f.size = BugEngine::uint2(1280,800);
    f.title = "discworld v0.01";
    f.border = true;
    f.fullscreen = false;
    f.vsync = false;
    f.triplebuffered = false;

    refptr<BugEngine::Graphics::Scene> scene = new Discworld::MainScene(app);
    app->createWindow(f, scene);

    return app->run();
}
/*---------------------------------------------------------------------------*/
