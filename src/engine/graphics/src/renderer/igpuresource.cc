/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/renderer/irenderer.hh>
#include    <graphics/renderer/igpuresource.hh>
#include    <system/resource/resource.script.hh>

namespace BugEngine { namespace Graphics
{

IGPUResource::IGPUResource(weak<const Resource> owner, weak<const IRenderer> renderer)
    :   m_renderer(renderer)
    ,   m_resource(owner)
    ,   m_index(-1)
{
}

IGPUResource::~IGPUResource()
{
}

}}
