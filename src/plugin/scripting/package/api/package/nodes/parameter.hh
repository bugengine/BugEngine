/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PACKAGE_NODES_PARAMETER_HH_
#define BE_PACKAGE_NODES_PARAMETER_HH_
/**************************************************************************************************/
#include    <package/stdafx.h>
#include    <rtti/typeinfo.script.hh>

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
    RTTI::Type::ConversionCost calculateConversion(const RTTI::Type& type) const;
    RTTI::Value as(const RTTI::Type& type) const;
};

}}}

/**************************************************************************************************/
#endif
