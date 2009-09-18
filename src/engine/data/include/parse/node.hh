/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_DATA_PARSE_NODE_HH_
#define BE_DATA_PARSE_NODE_HH_
/*****************************************************************************/

namespace BugEngine { namespace Data { namespace Parse
{

class Context;

class Node : public minitl::refcountable<void>
{
private:
    mutable Value  m_result;
private:
    virtual void dolink(Context& context) const = 0;
    virtual Value doeval(Context& context) const = 0;
public:
    Node();
    ~Node();

    void link(Context& context) const;
    Value eval(Context& context) const;
};

}}}

/*****************************************************************************/
#endif
