/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_3D_RENDERTARGET_RENDERTARGET_SCRIPT_HH_
#define BE_3D_RENDERTARGET_RENDERTARGET_SCRIPT_HH_
/**************************************************************************************************/
#include    <3d/stdafx.h>
#include    <resource/description.script.hh>

namespace BugEngine
{

class be_api(_3D) RenderTargetDescription : public Resource::Description
{
    BE_NOCOPY(RenderTargetDescription);
protected:
    RenderTargetDescription();
public:
    ~RenderTargetDescription();
};

class be_api(_3D) RenderSurfaceDescription : public RenderTargetDescription
{
    BE_NOCOPY(RenderSurfaceDescription);
published:
    const uint2 dimensions;
published:
    RenderSurfaceDescription(u16 width, u16 height);
    ~RenderSurfaceDescription();
};

class be_api(_3D) RenderWindowDescription : public RenderTargetDescription
{
    BE_NOCOPY(RenderWindowDescription);
published:
    const istring title;
published:
    RenderWindowDescription(istring title);
    ~RenderWindowDescription();
};

}

/**************************************************************************************************/
#endif
