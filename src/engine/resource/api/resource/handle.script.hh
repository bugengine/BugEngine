/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RESOURCE_HANDLE_HH_
#define BE_RESOURCE_HANDLE_HH_
/*****************************************************************************/

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
    u32 owner;
    Id  id;
};

}}

/*****************************************************************************/
#endif

