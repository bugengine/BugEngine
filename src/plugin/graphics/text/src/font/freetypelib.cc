/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.graphics.text/stdafx.h>
#include <freetypelib.hh>

namespace BugEngine {

FreetypeLibrary::FreetypeLibrary()
{
    FT_Error error = FT_Init_FreeType(&library);
    be_forceuse(error);
    be_assert(!error, "Freetype error %d" | error);
}

FreetypeLibrary::~FreetypeLibrary()
{
    FT_Done_FreeType(library);
}

}  // namespace BugEngine
