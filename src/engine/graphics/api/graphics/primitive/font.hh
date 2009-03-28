/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

#ifndef BE_GRAPHICS_FONT_FONT_HH_
#define BE_GRAPHICS_FONT_FONT_HH_
/*****************************************************************************/
#include    <graphics/renderer/vertexbuffer.hh>

#include    <ft2build.h>
#include    FT_FREETYPE_H

namespace BugEngine { namespace Graphics
{

class GRAPHICSEXPORT Font : public Object
{
private:
    struct VertexBuilder;

    FT_Face                                 m_face;
    typedef VertexDescription<MAKEVERTEXLIST2(float4, gPosition, float4, gUV0)> VertexFormat;
    refptr< VertexBuffer<VertexFormat> >    m_fontTriangles;
public:
    Font(const RenderBackend* renderer, const ifilename& font);
    virtual ~Font();

    const GpuBuffer* vertices() const;

    METACLASS(GRAPHICSEXPORT, Font, Object)
    PROPERTIES
    END
};


}}

/*****************************************************************************/
#endif
