/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_RUNTIME_CALLSTACK_HH_
#define BE_CORE_RUNTIME_CALLSTACK_HH_
/*****************************************************************************/

namespace BugEngine { namespace Runtime
{

class Callstack
{
public:
    class Address
    {
        friend class Callstack;
    private:
        u64 m_address;
    private:
        explicit Address(const void* address);
        explicit Address(u64 address);
    public:
        Address();
        Address(const Address& other);
        ~Address();
        Address& operator=(const Address& other);
        u64 address() const;
    };
public:
    static Address  backtrace(size_t depth);
    static size_t   backtrace(Address* buffer, size_t count, size_t skip);
};

}}

/*****************************************************************************/
#endif

