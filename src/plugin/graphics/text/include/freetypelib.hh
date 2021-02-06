/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_TEXT_FREETYPELIB_HH_
#define BE_TEXT_FREETYPELIB_HH_
/**************************************************************************************************/
#include <bugengine/plugin.graphics.text/stdafx.h>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace BugEngine {

class FreetypeLibrary : public minitl::pointer
{
public:
    FT_Library library;

public:
    FreetypeLibrary();
    ~FreetypeLibrary();
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
