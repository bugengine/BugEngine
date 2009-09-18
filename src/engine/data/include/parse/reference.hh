/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_DATA_PARSE_REFERENCE_HH_
#define BE_DATA_PARSE_REFERENCE_HH_
/*****************************************************************************/
#include <parse/node.hh>

namespace BugEngine { namespace Data { namespace Parse
{

class Reference : public Node
{
private:
    inamespace m_reference;
private:
    virtual void dolink(Context& context) const override;
    virtual Value doeval(Context& context) const override;
public:
    Reference(const inamespace& name);
    ~Reference();
};

}}}

/*****************************************************************************/
#endif
