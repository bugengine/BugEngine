/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/scene/scene.hh>

namespace BugEngine { namespace Graphics
{

Scene::Scene(Scene* parent)
:   m_parent(parent)
{
}

Scene::~Scene()
{
}

}}

