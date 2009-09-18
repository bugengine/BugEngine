/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <data/stdafx.h>
#include    <parse/node.hh>
#include    <parse/context.hh>

namespace BugEngine { namespace Data { namespace Parse
{

Node::Node()
{
}

Node::~Node()
{
}

void Node::link(Context& context) const
{
    return dolink(context);
}

Value Node::eval(Context& context) const
{
    if(m_result.type() == 0)
    {
        m_result = doeval(context);
    }
    return m_result;
}


}}}
