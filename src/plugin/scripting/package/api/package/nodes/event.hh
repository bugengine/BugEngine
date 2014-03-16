/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
 see LICENSE for detail */

#ifndef BE_PACKAGE_NODES_EVENT_HH_
#define BE_PACKAGE_NODES_EVENT_HH_
/**************************************************************************************************/
#include    <package/stdafx.h>


namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

class Event : public minitl::refcountable
{
public:
    Event();
    ~Event();
};

}}}

/**************************************************************************************************/
#endif
