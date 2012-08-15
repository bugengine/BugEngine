/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <3d/stdafx.h>
#include    <3d/renderer/irenderer.hh>
#include    <3d/renderer/igpuresource.hh>
#include    <resource/resource.script.hh>

namespace BugEngine
{

IGPUResource::IGPUResource(weak<const Resource> owner, weak<const IRenderer> renderer)
    :   m_renderer(renderer)
    ,   m_resource(owner)
    ,   m_index(-1)
{
}

IGPUResource::~IGPUResource()
{
    if (hooked()) unhook();
}

}
