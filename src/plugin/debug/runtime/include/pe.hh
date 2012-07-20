/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_DEBUG_RUNTIME_PE_HH_
#define BE_DEBUG_RUNTIME_PE_HH_
/*****************************************************************************/
#include <runtime/module.hh>

namespace BugEngine { namespace Runtime
{

class PE : public Module
{
    BE_NOCOPY(PE);
private:
    struct StringTable
    {
        u32     size;
        char    strings[1];
    };
private:
    FILE*                               m_file;
    SymbolResolver::SymbolInformations  m_symbolInformations;
public:
    PE(const char *filename, u64 baseAddress);
    ~PE();

    operator void*() const { return (void*) m_file; }
    bool operator !() const { return m_file != 0; }

    virtual Endianness endianness() const override { return Endianness_Little; }
    virtual SymbolResolver::SymbolInformations getSymbolInformation() const override;
};

}}

/*****************************************************************************/
#endif

