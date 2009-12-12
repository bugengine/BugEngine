/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>

#include    <main/main.hh>

#include    <mainscene.hh>

#include    <core/threads/thread.hh>

#include    <rtti/test.hh>
#include    <rtti/script.hh>
#include    <rtti/namespace.hh>

#include    <core/runtime/symbols.hh>

/*---------------------------------------------------------------------------*/
int be_main (weak<BugEngine::Application> app)
{
    printf("Test\n");
    ref<TestNS::Test> t = ref<TestNS::Test>::create();
    BugEngine::RTTI::Namespace::root()->insert("Sub1.Sub2.test", ref<BugEngine::Object>(t));
    BugEngine::Plugin<BugEngine::Script> p("lua");
    p->doFile("data/scripts/main.lua");
    printf("Done\n");

    BugEngine::Graphics::WindowFlags f;
    f.position = BugEngine::int2(0,0);
    f.size = BugEngine::uint2(1280,800);
    f.title = "discworld v0.01";
    f.border = true;
    f.fullscreen = false;
    f.vsync = false;
    f.triplebuffered = false;

    ref<BugEngine::Graphics::Scene> scene = ref<Discworld::MainScene>::create(app);
    app->createWindow(f, scene);

    return app->run();
}
/*---------------------------------------------------------------------------*/
