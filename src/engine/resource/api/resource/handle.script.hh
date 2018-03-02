/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RESOURCE_HANDLE_HH_
#define BE_RESOURCE_HANDLE_HH_
/**************************************************************************************************/
#include    <resource/stdafx.h>

namespace BugEngine { namespace Resource
{

class ILoader;

struct be_api(RESOURCE) Handle
{
    union Id
    {
        void*   ptrId;
        u32     intId;
    };
    Id  id;
    u32 owner;
};

}}

/**************************************************************************************************/
#endif

