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
published:
    enum TargetType { Window, Buffer };
private:
    uint2       m_dimensions;
    TargetType  m_type;
published:
    RenderTarget(TargetType type, u16 width, u16 height);
    ~RenderTarget();
};

}}

/*****************************************************************************/
#endif
