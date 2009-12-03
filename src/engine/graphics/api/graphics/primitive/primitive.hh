/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_PRIMITIVE_PRIMITIVE_HH_
#define BE_GRAPHICS_PRIMITIVE_PRIMITIVE_HH_
/*****************************************************************************/
#include    <graphics/renderer/vertexbuffer.hh>
#include    <graphics/renderer/indexbuffer.hh>

namespace BugEngine { namespace Graphics
{

class be_api(GRAPHICS) Primitive : public Object
{
public:
    Primitive(weak<const RenderBackend> renderer);
    ~Primitive();

    virtual weak<const GpuBuffer> indices() const = 0;
    virtual weak<const GpuBuffer> vertices() const = 0;
    virtual unsigned              nbVertices() const = 0;

    be_metaclass(GRAPHICS,Primitive,Object)
    be_properties
    be_end
};

}}

/*****************************************************************************/
#endif
