/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_RUNTIME_SYMBOLS_HH_
#define BE_RUNTIME_SYMBOLS_HH_
/*****************************************************************************/
#include    <runtime/callstack.hh>

namespace BugEngine { namespace Runtime
{

class Module;
class SymbolResolver;

class be_api(RUNTIME) Symbol
{
    friend class SymbolResolver;
    friend class Module;
private:
    char        m_module[4096];
    char        m_filename[4096];
    char        m_function[4096];
    u64         m_address;
    int         m_line;
public:
    Symbol() { *m_module = 0; *m_filename = 0; *m_function = 0; m_address = 0; m_line = 0; }
    u64         address() const     { return m_address; }
    const char *module() const      { return m_module; }
    const char *filename() const    { return m_filename; }
    int         line() const        { return m_line; }
    const char *function() const    { return m_function; }
};

class be_api(RUNTIME) SymbolResolver : public minitl::refcountable
{
    BE_NOCOPY(SymbolResolver);
private:
    ref<const SymbolResolver> m_next;
protected:
    static void fillSymbol(Symbol& symbol, u64 address, const char *module, const char *filename, const char *function, int line);
    virtual bool resolve(u64 address, Symbol& symbol) const = 0;
public:
    struct SymbolInformations
    {
        enum SymbolType
        {
            None,
            ELFDwarf,
            PEDwarf,
            Coff,
            PDB20,
            PDB70
        };

        SymbolType  type;
        ifilename   filename;
        istring     identifier;
        istring     section;
        u64         offset;
        u64         size;

        SymbolInformations() : type(None), filename(""), offset(0), size(0) { }
    };

    SymbolResolver();
    virtual ~SymbolResolver();
    bool resolve(Callstack::Address& address, Symbol& symbol) const;

    static ref<const SymbolResolver> loadSymbols(const SymbolInformations& infos, ref<const SymbolResolver> next);
};

}}

/*****************************************************************************/
#endif

