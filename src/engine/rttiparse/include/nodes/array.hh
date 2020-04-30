/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTIPARSE_NODES_ARRAY_HH_
#define BE_RTTIPARSE_NODES_ARRAY_HH_
/**************************************************************************************************/
#include <bugengine/rttiparse/stdafx.h>
#include <bugengine/rttiparse/valueparse.hh>

namespace BugEngine { namespace RTTI { namespace Parser {

class Array : public Node
{
private:
    const minitl::vector< ref< Node > > m_value;

protected:
    virtual bool resolve(DbContext& context) override;
    virtual bool isCompatible(const Type& expectedType) const override;
    virtual void doEval(const Type& expectedType, Value& result) const override;

public:
    Array(const ParseLocation& location, const minitl::vector< ref< Node > >& value);
    ~Array();
};

}}}  // namespace BugEngine::RTTI::Parser

/**************************************************************************************************/
#endif
