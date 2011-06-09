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


int be_main(weak<BugEngine::Application> app)
{
    ref<BugEngine::FileSystem> filesystem = ref<BugEngine::FileSystem>::create(BugEngine::gameArena());
    filesystem->mount("data", ref<BugEngine::DiskFS>::create(BugEngine::gameArena(), BugEngine::Environment::getEnvironment().getDataDirectory(), true));

    weak<const BugEngine::RTTI::Namespace> root = BugEngine::RTTI::Namespace::rttiRoot();

    BugEngine::Plugin<BugEngine::Scripting> p("lua", weak<BugEngine::FileSystem>(filesystem), BugEngine::Value(root));
    BugEngine::Plugin<BugEngine::Graphics::IRenderer> display1("renderNull",  weak<BugEngine::FileSystem>(filesystem));
    BugEngine::Plugin<BugEngine::Graphics::IRenderer> display2("renderOpenGL",  weak<BugEngine::FileSystem>(filesystem));
    BugEngine::Plugin<BugEngine::Graphics::IRenderer> display3("renderDx9", weak<BugEngine::FileSystem>(filesystem));

    p->doFile("data/scripts/main.lua");

    ref<BugEngine::Graphics::RenderWindow> w1 = ref<BugEngine::Graphics::RenderWindow>::create(BugEngine::gameArena(), (u16)800, (u16)600, "discworld v0.1", false);
    ref<BugEngine::World> world = ref<BugEngine::World>::create(BugEngine::gameArena(), "physicsBullet", "audioOpenAL", BugEngine::float3(1000.0f, 1000.0f, 1000.0f));
    ref<BugEngine::Graphics::IScene> scene = ref<BugEngine::WorldScene>::create(BugEngine::gameArena(), world);
    ref<BugEngine::Graphics::RenderScene> renderscene1 = ref<BugEngine::Graphics::RenderScene>::create(BugEngine::gameArena(), scene, w1);
    minitl::vector< ref<const BugEngine::Graphics::RenderNode> > scenes(BugEngine::gameArena());
    scenes.push_back(renderscene1);
    ref<BugEngine::Graphics::RenderSequence> node = ref<BugEngine::Graphics::RenderSequence>::create(BugEngine::gameArena(), scenes);

    BugEngine::Resource::load(BugEngine::Value(w1));
    BugEngine::Resource::load(BugEngine::Value(scene));
    BugEngine::Resource::load(BugEngine::Value(renderscene1));
    BugEngine::Resource::load(BugEngine::Value(node));

    app->run();

    BugEngine::Resource::unload(BugEngine::Value(node));
    BugEngine::Resource::unload(BugEngine::Value(renderscene1));
    BugEngine::Resource::unload(BugEngine::Value(scene));
    BugEngine::Resource::unload(BugEngine::Value(w1));

    return 0;
}
