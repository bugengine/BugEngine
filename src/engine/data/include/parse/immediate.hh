/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_DATA_PARSE_IMMEDIATE_HH_
#define BE_DATA_PARSE_IMMEDIATE_HH_
/*****************************************************************************/
#include <parse/node.hh>

namespace BugEngine { namespace Data { namespace Parse
{

class Immediate : public Node
{
private:
    Value  m_value;
private:
    virtual void dolink(Context& context) const override;
    virtual Value doeval(Context& context) const override;
public:
    Immediate(const Value& value);
    ~Immediate();
};

}}}

/*****************************************************************************/
#endif
