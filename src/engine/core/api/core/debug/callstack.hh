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

#ifndef BE_CORE_DEBUG_CALLSTACK_HH_
#define BE_CORE_DEBUG_CALLSTACK_HH_
/*****************************************************************************/

namespace BugEngine { namespace Debug
{

class Callstack
{
public:
    class Address
    {
        friend class Callstack;
        friend class SymbolResolver;
    private:
        void*                   m_address;
        mutable unsigned int    m_line;
        mutable char            m_filename[256];
        mutable char            m_function[256];
    private:
        explicit Address(void* address);
    private:
        void fill() const;
    public:
        Address();
        Address(const Address& other);
        ~Address();
        Address& operator=(const Address& other);

        const char *filename() const;
        unsigned int line() const;
        const char *function() const;
        void* pointer() const;
    };
public:
    static size_t   backtrace(Address* buffer, size_t count, size_t skip = 0);
    static Address  backtrace(size_t depth);
private:
    static size_t   backtrace(void** buffer, size_t count, size_t skip);
};

}}

/*****************************************************************************/
#endif

