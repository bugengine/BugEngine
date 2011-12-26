/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_3D_RENDERTARGET_RENDERTARGET_SCRIPT_HH_
#define BE_3D_RENDERTARGET_RENDERTARGET_SCRIPT_HH_
/*****************************************************************************/
#include    <system/resource/resource.script.hh>

namespace BugEngine { namespace Graphics
{

class be_api(_3D) RenderTarget : public Resource
{
    BE_NOCOPY(RenderTarget);
protected:
    RenderTarget();
public:
    ~RenderTarget();
};

class be_api(_3D) RenderSurface : public RenderTarget
{
    BE_NOCOPY(RenderSurface);
published:
    const uint2 dimensions;
published:
    RenderSurface(u16 width, u16 height);
    ~RenderSurface();
};

class be_api(_3D) RenderWindow : public RenderTarget
{
    BE_NOCOPY(RenderWindow);
published:
    const istring title;
published:
    RenderWindow(istring title);
    ~RenderWindow();
};

}}

/*****************************************************************************/
#endif
