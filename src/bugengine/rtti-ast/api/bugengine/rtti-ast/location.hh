/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_AST_LOCATION_HH_
#define BE_RTTI_AST_LOCATION_HH_
/**************************************************************************************************/
#include <bugengine/rtti-ast/stdafx.h>

namespace BugEngine { namespace RTTI { namespace AST {

struct ParseLocation
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

}}}  // namespace BugEngine::RTTI::AST

/**************************************************************************************************/
#endif
