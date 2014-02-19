/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_TEXT_FREETYPEFACE_HH_
#define BE_TEXT_FREETYPEFACE_HH_
/*****************************************************************************/
#include    <ft2build.h>
#include    <freetype.h>
#include    <minitl/allocator.hh>

namespace BugEngine
{

class FreetypeLibrary;

class FreetypeFace : public minitl::refcountable
{
public:
    FT_Face face;
public:
    FreetypeFace(weak<FreetypeLibrary> freetype, const minitl::Allocator::Block<u8>& buffer);
    ~FreetypeFace();
};

}

/*****************************************************************************/
#endif
