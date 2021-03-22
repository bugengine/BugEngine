/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_META_TAGS_DOCUMENTATION_SCRIPT_HH_
#define BE_META_TAGS_DOCUMENTATION_SCRIPT_HH_
/**************************************************************************************************/
#include <bugengine/meta/stdafx.h>

namespace BugEngine { namespace Meta {

struct be_api(META) Documentation
{
public:
    u64       size;
    const u8* bytes;
published:
    Documentation(u64 size, const u8* gzipBytes) : size(size), bytes(gzipBytes)
    {
    }
};

}}  // namespace BugEngine::Meta

/**************************************************************************************************/
#endif
