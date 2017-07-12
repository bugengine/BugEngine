/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PACKAGE_NODES_OVERLOADMATCH_HH_
#define BE_PACKAGE_NODES_OVERLOADMATCH_HH_
/**************************************************************************************************/
#include    <package/stdafx.h>
#include    <rtti/classinfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/engine/call.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

class Parameter;
class Object;

struct OverloadMatch
{
    friend class Object;
public:
    typedef BugEngine::RTTI::Method::Parameter RTTIParameter;
private:
    raw<const RTTI::Method::Overload>   m_overload;
    RTTI::Type::ConversionCost          m_cost;
public:
    OverloadMatch(raw<const RTTI::Method::Overload> overload);
    void update(const minitl::vector< ref<const Parameter> >& parameters);
    bool operator<(const OverloadMatch& other) const;
    RTTI::Value create(istring name, const minitl::vector<ref<const Parameter> >& parameters) const;
};

}}}

/**************************************************************************************************/
#endif
