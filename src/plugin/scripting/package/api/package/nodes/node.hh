/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
 see LICENSE for detail */

#ifndef BE_PACKAGE_NODES_NODE_HH_
#define BE_PACKAGE_NODES_NODE_HH_
/**************************************************************************************************/
#include    <package/stdafx.h>

namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

class Package;

class Node : public minitl::refcountable
{
protected:
    weak<Package> const     m_owner;
    u32 const               m_line;
    u32 const               m_begin;
    u32 const               m_end;
protected:
    Node(weak<Package> owner, u32 line, u32 begin, u32 end);
    ~Node();
};

}}}

/**************************************************************************************************/
#endif

