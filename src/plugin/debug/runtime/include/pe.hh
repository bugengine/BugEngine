/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_DEBUG_RUNTIME_PE_HH_
#define BE_DEBUG_RUNTIME_PE_HH_
/**************************************************************************************************/
#include <bugengine/plugin.debug.runtime/stdafx.h>
#include <bugengine/plugin.debug.runtime/module.hh>

namespace BugEngine { namespace Runtime {

class PE : public Module
{
    BE_NOCOPY(PE);

private:
    struct StringTable
    {
        u32  size;
        char strings[1];
    };

private:
    SymbolResolver::SymbolInformations m_symbolInformations;

public:
    PE(const char* filename, u64 baseAddress);
    ~PE();

    operator void*() const
    {
        return 0;
    }
    bool operator!() const
    {
        return 0;
    }

    virtual Endianness endianness() const override
    {
        return Endianness_Little;
    }
    virtual SymbolResolver::SymbolInformations getSymbolInformation() const override;
};

}}  // namespace BugEngine::Runtime

/**************************************************************************************************/
#endif
