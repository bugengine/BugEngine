/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_3D_RENDERTARGET_RENDERTARGET_SCRIPT_HH_
#define BE_3D_RENDERTARGET_RENDERTARGET_SCRIPT_HH_
/*****************************************************************************/
#include    <system/resource/resource.script.hh>

namespace BugEngine { namespace Graphics
{

be_tag(ResourceLoaders())
class be_api(_3D) RenderTarget : public Resource
{
    BE_NOCOPY(RenderTarget);
published:
    const uint2 dimensions;
published:
    RenderTarget(u16 width, u16 height);
    ~RenderTarget();
};

be_tag(ResourceLoaders())
class be_api(_3D) RenderWindow : public RenderTarget
{
    BE_NOCOPY(RenderWindow);
published:
    const istring title;
    const bool    fullscreen;
published:
    RenderWindow(u16 width, u16 height, istring title, bool fullscreen);
    ~RenderWindow();
};

}}

/*****************************************************************************/
#endif
