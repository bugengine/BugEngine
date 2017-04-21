/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTIPARSE_NODES_STRING_HH_
#define BE_RTTIPARSE_NODES_STRING_HH_
/**************************************************************************************************/
#include    <rttiparse/stdafx.h>
#include    <rttiparse/valueparse.hh>


namespace BugEngine { namespace RTTI { namespace Parser
{

class String : public Node
{
private:
    const char* m_value;
protected:
    virtual bool isCompatible(const Type& expectedType) const override;
    virtual void doEval(const Type& expectedType, Value& result) const override;
public:
    String(const ParseLocation& location, const char* value);
    ~String();
};

}}}

/**************************************************************************************************/
#endif
