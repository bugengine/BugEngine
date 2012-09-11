/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_PACKAGE_NODES_OVERLOADMATCH_HH_
#define BE_PACKAGE_NODES_OVERLOADMATCH_HH_
/*****************************************************************************/
#include    <rtti/classinfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

class Parameter;
class Object;

struct OverloadMatch
{
    friend class Object;
public:
    typedef BugEngine::RTTI::Method::Overload::Parameter RTTIParameter;
    struct ParameterMatch
    {
        weak<const Parameter>       parameter;
        raw<const RTTIParameter>    match;
    };
private:
    raw<const RTTI::Method::Overload>   m_overload;
    minitl::vector<ParameterMatch>      m_params;
    u32                                 m_score;
public:
    OverloadMatch(raw<const RTTI::Method::Overload> overload);
    void addParameter(weak<const Parameter> param);
    bool operator<(const OverloadMatch& other) const;
    RTTI::Value create(istring name) const;
};

}}}

/*****************************************************************************/
#endif
