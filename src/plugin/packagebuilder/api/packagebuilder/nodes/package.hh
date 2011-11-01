/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_PACKAGEBUILDER_NODES_PACKAGE_HH_
#define BE_PACKAGEBUILDER_NODES_PACKAGE_HH_
/*****************************************************************************/

namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

class Object;
class Package : public minitl::refcountable
{
private:
    minitl::vector< minitl::pair< istring, ref<Object> > >   m_nodes;
public:
    Package();
    ~Package();

};

}}}

/*****************************************************************************/
#endif
