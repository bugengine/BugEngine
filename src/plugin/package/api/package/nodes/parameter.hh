/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_PACKAGE_NODES_PARAMETER_HH_
#define BE_PACKAGE_NODES_PARAMETER_HH_
/*****************************************************************************/

namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

class Parameter : public minitl::refcountable
{
private:
    istring     m_name;
public:
    Parameter(istring name);
    ~Parameter();

    istring name() const    { return m_name; }
};

}}}

/*****************************************************************************/
#endif
