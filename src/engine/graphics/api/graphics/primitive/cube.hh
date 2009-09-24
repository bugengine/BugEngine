/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_PRIMITIVE_CUBE_HH_
#define BE_GRAPHICS_PRIMITIVE_CUBE_HH_
/*****************************************************************************/
#include    <graphics/primitive/primitive.hh>

namespace BugEngine { namespace Graphics
{

class be_api(GRAPHICS) Cube : public Primitive
{
private:
    typedef VertexDescription<MAKEVERTEXLIST1(float4, gPosition)> VertexFormat;
    refptr< VertexBuffer<VertexFormat> >    m_vertexBuffer;
    refptr< IndexBuffer<u16> >              m_indexBuffer;
    u32                                     m_totalVertices;
public:
    Cube(const RenderBackend* renderer);
    ~Cube();

    virtual const GpuBuffer* indices() const override;
    virtual const GpuBuffer* vertices() const override;
    virtual unsigned         nbVertices() const override;

    be_metaclass(GRAPHICS,Cube,Primitive)
    be_properties
    be_end
};

}}

/*****************************************************************************/
#endif
