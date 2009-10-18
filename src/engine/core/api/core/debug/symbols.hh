/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_DEBUG_SYMBOLS_HH_
#define BE_CORE_DEBUG_SYMBOLS_HH_
/*****************************************************************************/
#include    <core/debug/callstack.hh>

namespace BugEngine { namespace Debug
{

class DwarfModule;
class PDB;

class Symbols
{
public:
    class Symbol;
    class Module;
    class ISymbolResolver;
public:
    class Symbol
    {
        friend class Symbols;
        friend class Module;
        friend class DwarfModule;
        friend class PDB;
    private:
        char        m_module[4096];
        char        m_filename[4096];
        char        m_function[4096];
        const void* m_address;
        int         m_line;
    public:
        Symbol();
        ~Symbol();

        const void* address() const     { return m_address; }
        const char *module() const      { return m_module; }
        const char *filename() const    { return m_filename; }
        int         line() const        { return m_line; }
        const char *function() const    { return m_function; }
    };
public:
    class ISymbolResolver : public minitl::refcountable<void>
    {
    public:
        virtual ~ISymbolResolver() { }
        virtual bool resolve(u64 address, Symbol& result) const = 0;
    };
public:
    class Module
    {
    private:
        ifilename                               m_filename;
        u64                                     m_baseAddress;
        mutable refptr<const ISymbolResolver>   m_symbols;
    private:
        void loadDebugInformation() const;
    public:
        Module(const char* filename, u64 baseAddress);
        ~Module();

        static std::vector<Module> enumerate();
        bool resolve(const Callstack::Address& address, Symbol& result) const;
    };
private:
    enum _TargetSelf { Self };
    Symbols(_TargetSelf self);
public:
    Symbols();
    ~Symbols();

    void addModule(const Module& m);
    void resolve(const Callstack::Address& address, Symbol& result) const;

    static const Symbols& runningSymbols();
private:
    std::vector<Module> m_modules;
};

}}

/*****************************************************************************/
#endif

