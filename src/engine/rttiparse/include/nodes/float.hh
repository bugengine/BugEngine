/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTIPARSE_NODES_FLOAT_HH_
#define BE_RTTIPARSE_NODES_FLOAT_HH_
/**************************************************************************************************/
#include <bugengine/rttiparse/stdafx.h>
#include <bugengine/rttiparse/valueparse.hh>

namespace BugEngine { namespace RTTI { namespace Parser {

class Float : public Node
{
private:
    const double m_value;

protected:
    virtual bool isCompatible(const Type& expectedType) const override;
    virtual void doEval(const Type& expectedType, Value& result) const override;

public:
    Float(const ParseLocation& location, double value);
    ~Float();
};

}}}  // namespace BugEngine::RTTI::Parser

/**************************************************************************************************/
#endif
