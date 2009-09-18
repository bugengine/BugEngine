/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_PRIMITIVE_TEXT_HH_
#define BE_GRAPHICS_PRIMITIVE_TEXT_HH_
/*****************************************************************************/
#include    <graphics/primitive/primitive.hh>
#include    <graphics/primitive/font.hh>

namespace BugEngine { namespace Graphics
{

class GRAPHICSEXPORT Text : public Primitive
{
private:
    refptr<const Font>          m_font;
    refptr< IndexBuffer<u16> >  m_indexBuffer;
    int                         m_totalVertices;
public:
    Text(const RenderBackend* renderer, const Font* font, const char* m);
    ~Text();

    virtual const GpuBuffer* indices() const override;
    virtual const GpuBuffer* vertices() const override;
    virtual unsigned         nbVertices() const override;

    be_metaclass(GRAPHICSEXPORT,Text,Primitive)
    be_properties
    be_end
};

}}

/*****************************************************************************/
#endif
