/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graph/loader.hh>

#include    <graph/scenenode.hh>
#include    <graph/multinode.hh>
#include    <graphics/objects/scenegraph.script.hh>


namespace BugEngine { namespace Graphics
{

SceneGraphLoader::SceneGraphLoader()
{
    ResourceLoaders::attach<SceneGraph, SceneGraphLoader>(this, &SceneGraphLoader::load, &SceneGraphLoader::unload);
}

SceneGraphLoader::~SceneGraphLoader()
{
    ResourceLoaders::detach<SceneGraph, SceneGraphLoader>(this);
}

void* SceneGraphLoader::load(weak<const SceneGraph> /*source*/)
{
    return 0; //new Node
}

void SceneGraphLoader::unload(const void* /*resource*/)
{
}

}}
