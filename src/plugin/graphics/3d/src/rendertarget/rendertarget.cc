/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin.graphics.3d/stdafx.h>
#include <bugengine/plugin.graphics.3d/rendertarget/rendertarget.script.hh>

namespace BugEngine {

RenderTargetDescription::RenderTargetDescription()
{
}

RenderTargetDescription::~RenderTargetDescription()
{
}

RenderSurfaceDescription::RenderSurfaceDescription(u16 width, u16 height)
    : dimensions(make_uint2(width, height))
{
}

RenderSurfaceDescription::~RenderSurfaceDescription()
{
}

RenderWindowDescription::RenderWindowDescription(istring title) : title(title)
{
}

RenderWindowDescription::~RenderWindowDescription()
{
}

}  // namespace BugEngine
