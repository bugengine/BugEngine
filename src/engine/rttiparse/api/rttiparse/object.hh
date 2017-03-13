/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTIPARSE_OBJECT_HH_
#define BE_RTTIPARSE_OBJECT_HH_
/**************************************************************************************************/
#include    <rttiparse/stdafx.h>
#include    <rttiparse/valueparse.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <nodes/reference.hh>
#include    <nodes/parameter.hh>


namespace BugEngine { namespace RTTI { namespace Parser
{

class Object : public Node
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
