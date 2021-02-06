/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_TEXT_FREETYPEFACE_HH_
#define BE_TEXT_FREETYPEFACE_HH_
/**************************************************************************************************/
#include <bugengine/plugin.graphics.text/stdafx.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <bugengine/minitl/allocator.hh>

namespace BugEngine {

class FreetypeLibrary;

class FreetypeFace : public minitl::refcountable
{
public:
    FreetypeFace(weak< FreetypeLibrary > freetype, const minitl::Allocator::Block< u8 >& buffer);
    ~FreetypeFace();
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
