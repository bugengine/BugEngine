/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_AST_OBJECT_HH_
#define BE_RTTI_AST_OBJECT_HH_
/**************************************************************************************************/
#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/node.hh>
#include <bugengine/rtti/engine/methodinfo.script.hh>

namespace BugEngine { namespace RTTI { namespace AST {

class Reference;

class be_api(RTTI_AST) Parameter : public Node
{
private:
    istring     m_name;
    ref< Node > m_value;

public:
    Parameter(istring name, ref< Node > value);
    ~Parameter();

    virtual bool resolve(DbContext & context) override;
    virtual void doEval(const Type& expectedType, Value& result) const override;
    virtual bool isCompatible(const Type& expectedType) const override;

    const istring& name() const
    {
        return m_name;
    }
};

class be_api(RTTI_AST) Object : public Node
{
private:
    const ref< Reference >                   m_className;
    const minitl::vector< ref< Parameter > > m_parameters;

protected:
    virtual bool resolve(DbContext & context) override;
    virtual bool isCompatible(const Type& expectedType) const override;
    virtual void doEval(const Type& expectedType, Value& result) const override;

public:
    Object(ref< Reference > className, const minitl::vector< ref< Parameter > >& parameters);
    ~Object();
    Type getType() const;
};

}}}  // namespace BugEngine::RTTI::AST

/**************************************************************************************************/
#endif
