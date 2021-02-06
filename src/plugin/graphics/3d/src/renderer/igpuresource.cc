/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.graphics.3d/stdafx.h>
#include <bugengine/plugin.graphics.3d/renderer/igpuresource.hh>
#include <bugengine/plugin.graphics.3d/renderer/irenderer.hh>
#include <bugengine/resource/description.script.hh>

namespace BugEngine {

IGPUResource::IGPUResource(weak< const Resource::Description > owner,
                           weak< const IRenderer >             renderer)
    : m_renderer(renderer)
    , m_resource(owner)
    , m_index(-1)
{
}

IGPUResource::~IGPUResource()
{
    if(hooked()) unhook();
}

}  // namespace BugEngine
