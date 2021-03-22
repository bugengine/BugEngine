/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_META_PARSE_LOCATION_HH_
#define BE_META_PARSE_LOCATION_HH_
/**************************************************************************************************/
#include <bugengine/reflection/stdafx.h>

namespace BugEngine { namespace Meta { namespace Parse {

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

}}}  // namespace BugEngine::Meta::Parse

/**************************************************************************************************/
#endif
