/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/renderer/rendertarget.hh>
#include    <graphics/scene/scene.hh>

namespace BugEngine { namespace Graphics
{

RenderTarget::RenderTarget(ref<const Scene> scene)
:   m_scene(scene)
{
}

weak<const Scene> RenderTarget::scene() const
{
    return m_scene;
}

}}
