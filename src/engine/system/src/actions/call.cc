/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

#include    <system/stdafx.h>
#include    <actions/call.hh>

namespace BugEngine { namespace Actions
{

be_abstractmetaclass_impl("Actions",Call);

Call::Call() :
    m_method(0)
{
}

Call::Call(Object::MetaClass::Method* method) :
    m_method(method)
{
}

Call::~Call()
{
}

refptr<Call> Call::MetaClass::createWithMethod(Object::MetaClass::Method* method)
{
    return new Call(method);
}

void Call::initialize(ActionContext* /*context*/) const
{
    AssertNotReached();
}

bool Call::oneturn(ActionContext* context) const
{
    ActionContext::StackFrame* frame = context->thisframe();
    size_t stackspace = frame->args();
    Value* wrapper = &(*frame)[0];
    Value result = m_method->metaclass()->call(wrapper, stackspace);
    frame->popall();
    frame->push(result);
    return true;
}

}}
