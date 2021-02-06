/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RUNTIME_MODULE_HH_
#define BE_RUNTIME_MODULE_HH_
/**************************************************************************************************/
#include <bugengine/plugin.debug.runtime/stdafx.h>
#include <bugengine/core/endianness.hh>
#include <bugengine/plugin.debug.runtime/callstack.hh>
#include <bugengine/plugin.debug.runtime/symbols.hh>

namespace BugEngine { namespace Runtime {

class Symbol;
class SymbolResolver;

class be_api(RUNTIME) Module : public minitl::refcountable
{
    BE_NOCOPY(Module);

public:
    class Section
    {
        friend class Module;

    public:
        istring name;
        u64     offset;
        u64     size;
        u64     fileOffset;
        u64     fileSize;

        bool operator!() const
        {
            return fileSize == 0;
        }
        operator void*() const
        {
            return (void*)(intptr_t)fileSize;
        }
    };

private:
    friend class Section;

protected:
    ifilename                 m_filename;
    u64                       m_baseAddress;
    ref< const Module >       m_next;
    minitl::vector< Section > m_sections;

public:
    Module(const char* filename, u64 baseAddress);
    virtual ~Module();

    virtual Endianness endianness() const = 0;

    const Section& operator[](const istring& index) const;

    void readSection(const Section& section, void* data) const;

    virtual SymbolResolver::SymbolInformations getSymbolInformation() const = 0;

    static ref< const Module > self();
    weak< const Module >       next() const
    {
        return m_next;
    }
};

}}  // namespace BugEngine::Runtime

/**************************************************************************************************/
#endif
