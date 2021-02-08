/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_PARSE_LOCATION_HH_
#define BE_RTTI_PARSE_LOCATION_HH_
/**************************************************************************************************/
#include <bugengine/rtti-parse/stdafx.h>

namespace BugEngine { namespace RTTI { namespace Parse {

struct Location
{
    u32 line;
    u32 columnStart;
    u32 columnEnd;

    void newline()
    {
        line++;
        columnStart = 0;
        columnEnd   = 1;
    }
    void update(u32 column)
    {
        columnStart = columnEnd;
        columnEnd += column;
    }
};

}}}  // namespace BugEngine::RTTI::Parse

/**************************************************************************************************/
#endif
