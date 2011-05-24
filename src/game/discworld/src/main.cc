/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>

#include    <main/main.hh>

#include    <system/filesystem.hh>
#include    <system/diskfs.hh>
#include    <mobile/worldscene.script.hh>

#include    <graphics/renderer/irenderer.hh>

#include    <graphics/objects/scenegraph.script.hh>
#include    <graphics/objects/rendertarget.script.hh>
#include    <graphics/objects/shader.script.hh>
#include    <graphics/objects/shaders/method.script.hh>

#include    <mobile/world.script.hh>

#include    <rtti/namespace.script.hh>
#include    <rtti/scripting.hh>
#include    <rtti/typeinfo.hh>
#include    <rtti/value.inl>

static BugEngine::Value buildRenderTarget()
{
    ref<BugEngine::Graphics::RenderTarget> result = ref<BugEngine::Graphics::RenderTarget>::create(BugEngine::gameArena(), 800, 600, "discworld v0.1", false);
    return BugEngine::Value(result);
}

static BugEngine::Value buildRenderTree()
{
}

static BugEngine::Value buildShader()
{
    ref<BugEngine::Graphics::Shader> result = ref<BugEngine::Graphics::Shader>::create(BugEngine::gameArena(), BugEngine::Graphics::Shader::Vertex, ref<BugEngine::Graphics::Shaders::Method>());
    return BugEngine::Value(result);
}

int be_main(weak<BugEngine::Application> app)
{
    ref<BugEngine::FileSystem> filesystem = ref<BugEngine::FileSystem>::create(BugEngine::gameArena());
    filesystem->mount("data", ref<BugEngine::DiskFS>::create(BugEngine::gameArena(), BugEngine::Environment::getEnvironment().getDataDirectory(), true));

    weak<const BugEngine::RTTI::Namespace> root = BugEngine::RTTI::Namespace::rttiRoot();
    BugEngine::Plugin<BugEngine::Scripting> p("lua", weak<BugEngine::FileSystem>(filesystem), BugEngine::Value(root));
    p->doFile("data/scripts/main.lua");

    //BugEngine::Plugin<BugEngine::Graphics::IRenderer> display("renderNull",  weak<BugEngine::FileSystem>(filesystem));
    BugEngine::Plugin<BugEngine::Graphics::IRenderer> display("renderOpenGL",  weak<BugEngine::FileSystem>(filesystem));
    //BugEngine::Plugin<BugEngine::Graphics::IRenderer> display2("renderDx9", weak<BugEngine::FileSystem>(filesystem));


    //scoped<BugEngine::Graphics::MultiNode> node = scoped<BugEngine::Graphics::MultiNode>::create(BugEngine::taskArena());
    {
        //ref<BugEngine::World> world = ref<BugEngine::World>::create(BugEngine::taskArena(), "physicsBullet", "audioOpenAL", BugEngine::float3(1000.0f, 1000.0f, 1000.0f));
        //ref<BugEngine::Graphics::IScene> scene = ref<BugEngine::WorldScene>::create(BugEngine::taskArena(), world);

        //node->addNode(scoped<BugEngine::Graphics::SceneNode>::create(BugEngine::taskArena(), scene, w1), BugEngine::Graphics::MultiNode::MainWindow);
        //node->addNode(scoped<BugEngine::Graphics::SceneNode>::create(BugEngine::taskArena(), scene, w2), BugEngine::Graphics::MultiNode::MainWindow);
        //node->addNode(scoped<BugEngine::Graphics::SceneNode>::create(BugEngine::taskArena(), scene, w3), BugEngine::Graphics::MultiNode::MainWindow);
        //node->addNode(scoped<BugEngine::Graphics::SceneNode>::create(BugEngine::taskArena(), scene, w4), BugEngine::Graphics::MultiNode::ToolWindow);
        //node->addNode(scoped<BugEngine::Graphics::SceneNode>::create(BugEngine::taskArena(), scene, w5), BugEngine::Graphics::MultiNode::ToolWindow);
        //app->setScene(scoped<BugEngine::Graphics::SceneNode>::create(taskArena(), scene, w));
        //app->setScene(node);
    }

    BugEngine::Value window = buildRenderTarget();
    BugEngine::Resource::load(window);
    BugEngine::Value shader = buildShader();
    BugEngine::Resource::load(shader);

    return app->run();
}
