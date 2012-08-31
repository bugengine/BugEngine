/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_3D_RENDERTARGET_RENDERTARGET_SCRIPT_HH_
#define BE_3D_RENDERTARGET_RENDERTARGET_SCRIPT_HH_
/*****************************************************************************/
#include    <resource/description.script.hh>

namespace BugEngine
{

class be_api(_3D) RenderTarget : public Resource::Description
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

}

/*****************************************************************************/
#endif
