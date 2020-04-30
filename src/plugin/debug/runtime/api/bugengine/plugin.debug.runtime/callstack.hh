/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RUNTIME_CALLSTACK_HH_
#define BE_RUNTIME_CALLSTACK_HH_
/**************************************************************************************************/
#include <bugengine/plugin.debug.runtime/stdafx.h>

namespace BugEngine { namespace Runtime {

class be_api(RUNTIME) Callstack
{
public:
    class be_api(RUNTIME) Address
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
        u64      address() const;
    };

public:
    static Address backtrace(size_t depth);
    static size_t  backtrace(Address * buffer, size_t count, size_t skip);
};

}}  // namespace BugEngine::Runtime

/**************************************************************************************************/
#endif
