/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RESOURCE_HANDLE_HH_
#define BE_RESOURCE_HANDLE_HH_
/**************************************************************************************************/
#include <bugengine/resource/stdafx.h>

namespace BugEngine { namespace Resource {

class ILoader;

struct be_api(RESOURCE) Handle
{
    union Id
    {
        void* ptrId;
        u32   intId;
    };
    Id  id;
    u32 owner;
};

}}  // namespace BugEngine::Resource

/**************************************************************************************************/
#endif
