/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <graphics/stdafx.h>
#include    <graphics/primitive/font.hh>
#include    <system/filesystem.hh>
#include    <core/memory/streams.hh>

#include    FT_GLYPH_H
#include    FT_OUTLINE_H

namespace BugEngine { namespace Graphics
{

be_abstractmetaclass_impl("Graphics",Font);

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

Font::Font(const RenderBackend* /*renderer*/, const ifilename& font)
{
    ref<IMemoryStream> fontFile = FileSystem::instance()->open(font, eReadOnly);
    FT_New_Memory_Face(g_library.m_library, (FT_Byte*)fontFile->memory(), be_checked_numcast<FT_Long>(fontFile->size()), 0, &m_face);

    if(m_face->face_flags & FT_FACE_FLAG_SCALABLE)
    {
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
                    break;
                }
            }
        }
    }
    else
    {
        /* TODO : generate texture */
    }
}

Font::~Font()
{
    FT_Done_Face(m_face);
}

weak<const GpuBuffer> Font::vertices() const
{
    return m_fontTriangles->buffer();
}

}}
