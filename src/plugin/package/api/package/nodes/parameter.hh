/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_PACKAGE_NODES_PARAMETER_HH_
#define BE_PACKAGE_NODES_PARAMETER_HH_
/*****************************************************************************/

namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

class Value;
class Parameter : public minitl::refcountable
{
private:
    istring     m_name;
    ref<Value>  m_value;
public:
    Parameter(istring name, ref<Value> value);
    ~Parameter();

    istring name() const;
    bool isCompatible(const TypeInfo& type) const;
};

}}}

/*****************************************************************************/
#endif
