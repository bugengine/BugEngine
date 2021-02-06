/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.graphics.text/stdafx.h>
#include <freetypeface.hh>
#include <freetypelib.hh>
#include FT_OUTLINE_H

namespace BugEngine {

struct OutlineDecompose
{
    static int moveTo(const FT_Vector* target, void* decompose)
    {
        be_forceuse(target);
        be_forceuse(decompose);
        return 0;
    }
    static int lineTo(const FT_Vector* target, void* decompose)
    {
        be_forceuse(target);
        be_forceuse(decompose);
        return 0;
    }
    static int conicTo(const FT_Vector* control, const FT_Vector* target, void* decompose)
    {
        be_forceuse(control);
        be_forceuse(target);
        be_forceuse(decompose);
        return 0;
    }
    static int cubicTo(const FT_Vector* control1, const FT_Vector* control2,
                       const FT_Vector* target, void* decompose)
    {
        be_forceuse(control1);
        be_forceuse(control2);
        be_forceuse(target);
        be_forceuse(decompose);
        return 0;
    }
};

FreetypeFace::FreetypeFace(weak< FreetypeLibrary >               freetype,
                           const minitl::Allocator::Block< u8 >& buffer)
{
    FT_Face  face = 0;
    FT_Error error;
    error = FT_New_Memory_Face(freetype->library, buffer.begin(),
                               be_checked_numcast< u32 >(buffer.byteCount()), 0, &face);
    be_forceuse(error);
    be_assert(!error, "Freetype error %d" | error);
    error = FT_Select_Charmap(face, FT_ENCODING_UNICODE);
    be_assert(!error, "Freetype error %d" | error);
    FT_UInt  glyphIndex = 0;
    FT_ULong charcode   = FT_Get_First_Char(face, &glyphIndex);
    while(glyphIndex)
    {
        error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_NO_BITMAP);
        if(!error && face->glyph->format == FT_GLYPH_FORMAT_OUTLINE)
        {
            FT_Outline_Funcs funcs;
            funcs.move_to  = &OutlineDecompose::moveTo;
            funcs.line_to  = &OutlineDecompose::lineTo;
            funcs.conic_to = &OutlineDecompose::conicTo;
            funcs.cubic_to = &OutlineDecompose::cubicTo;
            funcs.delta    = 0;
            funcs.shift    = 0;
            OutlineDecompose decompose;
            FT_Outline_Decompose(&face->glyph->outline, &funcs, (void*)&decompose);
        }
        charcode = FT_Get_Next_Char(face, charcode, &glyphIndex);
    }
    FT_Done_Face(face);
}

FreetypeFace::~FreetypeFace()
{
}

}  // namespace BugEngine
