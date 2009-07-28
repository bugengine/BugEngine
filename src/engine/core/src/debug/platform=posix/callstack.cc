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

#include    <core/stdafx.h>
#include    <core/debug/callstack.hh>


namespace BugEngine
{

Callstack::Address::Address()
:   m_address(0)
,   m_line(0)
{
}

Callstack::Address::Address(void* address)
:   m_address(address)
,   m_line(0)
{
}

Callstack::Address::Address(const Address& other)
:   m_address(other.m_address)
,   m_line(other.m_line)
{
    strncpy(m_filename, other.m_filename, sizeof(m_filename));
    strncpy(m_function, other.m_function, sizeof(m_function));
}

Callstack::Address& Callstack::Address::operator=(const Address& other)
{
    if(&other != this)
    {
        m_address = other.m_address;
        m_line = other.m_line;
        strncpy(m_filename, other.m_filename, sizeof(m_filename));
        strncpy(m_function, other.m_function, sizeof(m_function));
    }
    return *this;
}

Callstack::Address::~Address()
{
}

void Callstack::Address::fill() const
{
}

const char * Callstack::Address::filename() const
{
    fill();
    return m_filename;
}

const char * Callstack::Address::function() const
{
    fill();
    return m_function;
}

unsigned int Callstack::Address::line() const
{
    fill();
    return m_line;
}

void* Callstack::Address::pointer() const
{
    return m_address;
}

BE_NOINLINE size_t Callstack::backtrace(Address* buffer, size_t count, size_t skip)
{
    return 0;
}

BE_NOINLINE Callstack::Address Callstack::backtrace(size_t depth)
{
    return Address();
}

}
