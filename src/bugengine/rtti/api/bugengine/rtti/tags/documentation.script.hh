/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_TAGS_DOCUMENTATION_SCRIPT_HH_
#define BE_RTTI_TAGS_DOCUMENTATION_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/rtti/stdafx.h>

namespace BugEngine { namespace RTTI {

struct be_api(RTTI) Documentation
{
public:
    u64       size;
    const u8* bytes;
published:
    Documentation(u64 size, const u8* gzipBytes) : size(size), bytes(gzipBytes)
    {
    }
};

}}  // namespace BugEngine::RTTI

/**************************************************************************************************/
#endif
