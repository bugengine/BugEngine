/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_TEXT_FREETYPELIB_HH_
#define BE_TEXT_FREETYPELIB_HH_
/**************************************************************************************************/
#include    <text/stdafx.h>
#include    <ft2build.h>
#include    <freetype.h>

namespace BugEngine
{

class FreetypeLibrary : public minitl::pointer
{
public:
    FT_Library library;
public:
    FreetypeLibrary();
    ~FreetypeLibrary();
};

}

/**************************************************************************************************/
#endif
