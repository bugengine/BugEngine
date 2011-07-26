/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_OBJECTS_RENDERTARGET_HH_
#define BE_GRAPHICS_OBJECTS_RENDERTARGET_HH_
/*****************************************************************************/
#include    <system/resource/resource.script.hh>

namespace BugEngine { namespace Graphics
{

be_tag(ResourceLoaders())
class be_api(GRAPHICS) RenderTarget : public Resource
{
    BE_NOCOPY(RenderTarget);
published:
    const uint2 dimensions;
published:
    RenderTarget(u16 width, u16 height);
    ~RenderTarget();
};

be_tag(ResourceLoaders())
class be_api(GRAPHICS) RenderWindow : public RenderTarget
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
