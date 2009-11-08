/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>

#include    <core/runtime/callstack.hh>


namespace BugEngine { namespace Runtime
{

Callstack::Address::Address()
:   m_address(0)
{
}

Callstack::Address::Address(const void* address)
:   m_address(address)
{
}

Callstack::Address::Address(const Address& other)
:   m_address(other.m_address)
{
}

Callstack::Address& Callstack::Address::operator=(const Address& other)
{
    if(&other != this)
    {
        m_address = other.m_address;
    }
    return *this;
}

Callstack::Address::~Address()
{
}

const void* Callstack::Address::pointer() const
{
    return m_address;
}

BE_NOINLINE Callstack::Address Callstack::backtrace(size_t depth)
{
    Address result;
    backtrace(&result, 1, depth+1);
    return result;
}

}}
