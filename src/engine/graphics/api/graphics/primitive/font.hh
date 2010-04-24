/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_FONT_FONT_HH_
#define BE_GRAPHICS_FONT_FONT_HH_
/*****************************************************************************/
#include    <graphics/renderer/vertexbuffer.hh>
#include    <core/memory/streams.hh>

#include    <ft2build.h>
#include    FT_FREETYPE_H

namespace BugEngine { namespace Graphics
{

class be_api(GRAPHICS) Font : public Object
{
private:
    struct VertexBuilder;

    FT_Face                                 m_face;
    typedef VertexDescription<MAKEVERTEXLIST2(float4, gPosition, float4, gUV0)> VertexFormat;
    ref< VertexBuffer<VertexFormat> >    m_fontTriangles;
public:
    Font(const RenderBackend* renderer, ref<IMemoryStream> font);
    virtual ~Font();

    weak<const GpuBuffer> vertices() const;

    be_metaclass(GRAPHICS, Font, Object)
    be_properties
    be_end
};


}}

/*****************************************************************************/
#endif
