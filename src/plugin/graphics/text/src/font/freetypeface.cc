/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <text/stdafx.h>
#include    <freetypeface.hh>
#include    <freetypelib.hh>

namespace BugEngine
{

FreetypeFace::FreetypeFace(weak<FreetypeLibrary> freetype, const minitl::Allocator::Block<u8>& buffer)
{
    FT_Error error = FT_New_Memory_Face(freetype->library, buffer.begin(),
                                        be_checked_numcast<u32>(buffer.byteCount()),
                                        0,
                                        &face);
    be_assert(!error, "Freetype error %d" | error);
}

FreetypeFace::~FreetypeFace()
{
    FT_Done_Face(face);
}

}
