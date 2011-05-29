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
}

SceneGraphLoader::~SceneGraphLoader()
{
}

void* SceneGraphLoader::load(weak<const Resource> /*source*/)
{
    return 0; //new Node
}

void SceneGraphLoader::unload(const void* /*resource*/)
{
}

}}
