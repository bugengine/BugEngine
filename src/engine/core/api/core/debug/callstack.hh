/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

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
    private:
        const void* m_address;
    private:
        explicit Address(const void* address);
    public:
        Address();
        Address(const Address& other);
        ~Address();
        Address& operator=(const Address& other);
        const void* pointer() const;
    };
public:
    static Address  backtrace(size_t depth);
    static size_t   backtrace(Address* buffer, size_t count, size_t skip);
};

}}

/*****************************************************************************/
#endif

