/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
 see LICENSE for detail */

#include    <package/stdafx.h>
#include    <package/nodes/node.hh>
#include    <package/nodes/package.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

Node::Node(weak<Package> owner, u32 line, u32 begin, u32 end)
    :   m_owner(owner)
    ,   m_line(line)
    ,   m_begin(begin)
    ,   m_end(end)
{
}

Node::~Node()
{
}

}}}
