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
#include    <graphics/objects/shaders/float.script.hh>

#include    <mobile/world.script.hh>

#include    <rtti/namespace.script.hh>
#include    <rtti/scripting.hh>
#include    <rtti/typeinfo.hh>
#include    <rtti/value.inl>


int be_main(weak<BugEngine::Application> app)
{
    using namespace BugEngine;
    using namespace BugEngine::Graphics;

    ref<FileSystem> filesystem = ref<FileSystem>::create(gameArena());
    filesystem->mount("data", ref<DiskFS>::create(gameArena(), Environment::getEnvironment().getDataDirectory(), true));

    weak<const RTTI::Namespace> root = RTTI::Namespace::rttiRoot();

    Plugin<Scripting> p("lua", weak<FileSystem>(filesystem), Value(root));
    Plugin<IRenderer> display1("renderNull",  weak<FileSystem>(filesystem));
    Plugin<IRenderer> display2("renderOpenGL",  weak<FileSystem>(filesystem));
    Plugin<IRenderer> display3("renderDx9", weak<FileSystem>(filesystem));

    p->doFile("data/scripts/main.lua");


    ref<Shaders::Float4> vertexpos = ref<Shaders::Float4Attribute>::create(gameArena());
    ref<Shaders::Float> depth = ref<Shaders::FloatUniform>::create(gameArena(), "depth");
    ref<ShaderProgram> program = ref<ShaderProgram>::create(gameArena(), vertexpos, vertexpos, depth);

    ref<RenderWindow> w1 = ref<RenderWindow>::create(gameArena(), (u16)800, (u16)600, "discworld v0.1", false);
    ref<World> world = ref<World>::create(gameArena(), "physicsBullet", "audioOpenAL", float3(1000.0f, 1000.0f, 1000.0f));
    ref<IScene> scene = ref<WorldScene>::create(gameArena(), world);
    ref<RenderScene> renderscene1 = ref<RenderScene>::create(gameArena(), scene, w1);
    minitl::vector< ref<const RenderNode> > scenes(gameArena());
    scenes.push_back(renderscene1);
    ref<RenderSequence> node = ref<RenderSequence>::create(gameArena(), scenes);
    
    Resource::load(Value(program));
    Resource::load(Value(w1));
    Resource::load(Value(scene));
    Resource::load(Value(renderscene1));
    Resource::load(Value(node));

    app->run();

    Resource::unload(Value(node));
    Resource::unload(Value(renderscene1));
    Resource::unload(Value(scene));
    Resource::unload(Value(w1));
    Resource::unload(Value(program));

    return 0;
}
