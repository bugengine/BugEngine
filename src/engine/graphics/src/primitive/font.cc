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

#include    <graphics/stdafx.h>
#include    <graphics/primitive/font.hh>
#include    <system/filesystem.hh>
#include    <core/memory/streams.hh>

#include    FT_GLYPH_H
#include    FT_OUTLINE_H

namespace BugEngine { namespace Graphics
{

METACLASS_IMPL("Graphics",Font);

struct FreetypeLibrary
{
    FT_Library m_library;

    FreetypeLibrary()
    {
        FT_Init_FreeType(&m_library);
    }
    ~FreetypeLibrary()
    {
        FT_Done_FreeType(m_library);
    }
};

static FreetypeLibrary  g_library;

struct Font::VertexBuilder
{
    VertexFormat*   vertices;
    i32             index;
    i32             capacity;

    VertexBuilder()
        :   vertices(0)
        ,   index(0)
        ,   capacity(0)
    {
    }

    ~VertexBuilder()
    {
        be_free(vertices);
    }
};

Font::Font(const RenderBackend* renderer, const ifilename& font)
{
    refptr<AbstractMemoryStream> fontFile = FileSystem::instance()->open(font, eReadOnly);
    FT_New_Memory_Face(g_library.m_library, (FT_Byte*)fontFile->memory(), checked_numcast<FT_Long>(fontFile->size()), 0, &m_face);
    OutputDebugString(font.str().c_str());
    OutputDebugString(": ");

    int numVertices = 0;
    int vertexCapacity = 0;
    VertexFormat* vertices = 0;

    if(m_face->face_flags & FT_FACE_FLAG_SCALABLE)
    {
        OutputDebugString("outline font\n");
        for(unsigned int i = 0; i < (unsigned)m_face->num_glyphs; ++i)
        {
            if(! FT_Load_Glyph(m_face, i, FT_LOAD_NO_SCALE|FT_LOAD_NO_BITMAP))
            {
                switch(m_face->glyph->format)
                {
                case FT_GLYPH_FORMAT_OUTLINE:
                    {
                        
                    }
                    break;
                case FT_GLYPH_FORMAT_COMPOSITE:
                default:
                    OutputDebugString("unsupported format : ??\n");
                    break;
                }
            }
        }
    }
    else
    {
        OutputDebugString("bitmap font\n");
        /* TODO : generate texture */
    }
}

Font::~Font()
{
    FT_Done_Face(m_face);
}

const GpuBuffer* Font::vertices() const
{
    return m_fontTriangles->buffer();
}

}}
