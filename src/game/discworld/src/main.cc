/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>

#include    <main/main.hh>

#include    <core/threads/thread.hh>

#include    <system/filesystem.hh>
#include    <system/diskfs.hh>
#include    <mobile/worldscene.hh>

#include    <graphics/renderer/renderbackend.hh>
#include    <graphics/renderer/graph/scenenode.hh>
#include    <graphics/renderer/graph/multinode.hh>


#include    <mobile/world.hh>

/*---------------------------------------------------------------------------*/
int be_main (weak<BugEngine::Application> app)
{
    ref<BugEngine::FileSystem> filesystem = ref<BugEngine::FileSystem>::create();
    filesystem->mount("data", ref<BugEngine::DiskFS>::create(BugEngine::Environment::getEnvironment().getDataDirectory(), true));

    //ref<TestNS::Test> t = ref<TestNS::Test>::create();
    //BugEngine::RTTI::Namespace::root()->insert("Sub1.Sub2.test", ref<BugEngine::Object>(t));
    //BugEngine::Plugin<BugEngine::Script> p("lua", filesystem);
    //p->doFile("data/scripts/main.lua");

    BugEngine::Graphics::WindowFlags f;
    f.position = BugEngine::int2(0,0);
    f.size = BugEngine::uint2(480, 480);
    f.title = "discworld v0.01";
    f.border = true;
    f.fullscreen = false;
    f.vsync = false;
    f.triplebuffered = false;

    BugEngine::Plugin<BugEngine::Graphics::RenderBackend> display2("renderOpenGL", weak<BugEngine::FileSystem>(filesystem));
    BugEngine::Plugin<BugEngine::Graphics::RenderBackend> display("renderDx9", weak<BugEngine::FileSystem>(filesystem));

    scoped<BugEngine::Graphics::MultiNode> node = scoped<BugEngine::Graphics::MultiNode>::create();
    {

        ref<BugEngine::Graphics::IRenderTarget> w = display->createRenderWindow(f);
        f.position += BugEngine::int2(100, 100);
        ref<BugEngine::Graphics::IRenderTarget> w2 = display->createRenderWindow(f);
        f.position += BugEngine::int2(100, 100);
        ref<BugEngine::Graphics::IRenderTarget> w3 = display2->createRenderWindow(f);
        f.position += BugEngine::int2(100, 100);
        ref<BugEngine::Graphics::IRenderTarget> w4 = display2->createRenderWindow(f);
        f.position += BugEngine::int2(100, 100);
        ref<BugEngine::Graphics::IRenderTarget> w5 = display->createRenderWindow(f);

        //ref<BugEngine::Graphics::IRenderTarget> gbuffer = display->createRenderBuffer(f);

        ref<BugEngine::World> world = ref<BugEngine::World>::create("physicsBullet", "audioOpenAL", BugEngine::float3(1000.0f, 1000.0f, 1000.0f));
        ref<BugEngine::Graphics::IScene> scene = ref<BugEngine::WorldScene>::create(world);

        node->addNode(scoped<BugEngine::Graphics::SceneNode>::create(scene, w), BugEngine::Graphics::MultiNode::MainWindow);
        node->addNode(scoped<BugEngine::Graphics::SceneNode>::create(scene, w2), BugEngine::Graphics::MultiNode::MainWindow);
        node->addNode(scoped<BugEngine::Graphics::SceneNode>::create(scene, w3), BugEngine::Graphics::MultiNode::MainWindow);
        node->addNode(scoped<BugEngine::Graphics::SceneNode>::create(scene, w4), BugEngine::Graphics::MultiNode::ToolWindow);
        node->addNode(scoped<BugEngine::Graphics::SceneNode>::create(scene, w5), BugEngine::Graphics::MultiNode::ToolWindow);
        //app->setScene(scoped<BugEngine::Graphics::SceneNode>::create(scene, w));
        app->setScene(node);
    }

    int result = app->run();
    return result;
}
/*---------------------------------------------------------------------------*/
