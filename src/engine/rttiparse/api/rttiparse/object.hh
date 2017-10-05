/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTIPARSE_OBJECT_HH_
#define BE_RTTIPARSE_OBJECT_HH_
/**************************************************************************************************/
#include    <rttiparse/stdafx.h>
#include    <rttiparse/node.hh>
#include    <rtti/engine/methodinfo.script.hh>


namespace BugEngine { namespace RTTI { namespace Parser
{

class Reference;

struct Parameter
{
    ParseLocation   location;
    istring         name;
    ref<Node>       value;
};

class be_api(RTTIPARSE) Object : public Node
{
private:
    const ref<Reference>            m_className;
    const minitl::vector<Parameter> m_parameters;
protected:
    virtual bool resolve(DbContext& context) override;
    virtual bool isCompatible(const Type& expectedType) const override;
    virtual void doEval(const Type& expectedType, Value& result) const override;
public:
    Object(const ParseLocation& location,
           ref<Reference> className, const minitl::vector<Parameter>& parameters);
    ~Object();
    Type getType() const;
};

}}}

/**************************************************************************************************/
#endif
