/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>

#include    <core/debug/symbols.hh>

#ifdef BE_PLATFORM_POSIX
# include   <dlfcn.h>
# include   <link.h>
#endif
#ifdef BE_PLATFORM_WIN32
# include   <psapi.h>
#endif

#include    <elf.hh>
#include    <pe.hh>


namespace BugEngine { namespace Debug
{

Symbols::Symbol::Symbol()
:   m_line(0)
{
    m_module[0] = 0;
    m_filename[0] = 0;
    m_function[0] = 0;
}

Symbols::Symbol::~Symbol()
{
}

//---------------------------------------------------------------------------//

Symbols::Module::Module(const char *filename, u64 baseAddress)
:   m_filename(filename)
,   m_baseAddress(baseAddress)
{
}

Symbols::Module::~Module()
{
}

std::vector<Symbols::Module> Symbols::Module::enumerate()
{
    std::vector<Symbols::Module> modules;
#ifdef BE_PLATFORM_POSIX
    void* handle = dlopen(0, RTLD_LAZY);
    link_map* lmap;
    dlinfo(handle, RTLD_DI_LINKMAP, &lmap);
    for(int i = 0; lmap; lmap=lmap->l_next, i++)
    {
        if(i == 0 && !lmap->l_name)
        {
            /* main executable */
            /* filename seems broken */
            FILE* cmdline = fopen("/proc/self/cmdline", "r");
            if(!cmdline)
                continue;
            char filename[4096];
            fread(filename, 1, 4096, cmdline);
            modules.push_back(Module(filename, lmap->l_addr));
        }
        else
        {
            modules.push_back(Module(lmap->l_name, lmap->l_addr));
        }
    }
#elif defined(BE_PLATFORM_WIN32)
    HANDLE process = ::GetCurrentProcess();
    DWORD requiredSize;
    ::EnumProcessModules(process, 0, 0, &requiredSize);
    size_t moduleCount = requiredSize/sizeof(HMODULE);
    Malloc::MemoryBlock<HMODULE> hmodules(moduleCount);
    ::EnumProcessModules(process, hmodules, requiredSize, &requiredSize);
    for(size_t i = 0; i < requiredSize/sizeof(HMODULE); i++)
    {
        char moduleName[32768];
        MODULEINFO info;
        ::GetModuleFileNameEx(process, hmodules[i], moduleName, sizeof(moduleName));
        ::GetModuleInformation(process, hmodules[i], &info, sizeof(info));
        modules.push_back(Module(moduleName, (u64)info.lpBaseOfDll));
    }
#else
# error platform not supported yet...
#endif
    return modules;
}

void Symbols::Module::loadDebugInformation() const
{
    FILE* f = fopen(m_filename.str().c_str(), "rb");
    if(f)
    {
        char signature[2];
        fread(signature, 1, 2, f);
        fclose(f);
        if (signature[0] == 'M' && signature[1] == 'Z')
        {
            m_symbols = PE(m_filename).getSymbolResolver();
        }
        else if (signature[0] == 0x7f && signature[1] == 'E')
        {
            m_symbols = Elf(m_filename).getSymbolResolver();
        }
    }
}

bool Symbols::Module::resolve(const Callstack::Address& address, Symbol& result) const
{
    if(!m_symbols)
    {
        loadDebugInformation();
        be_assert(m_symbols, "Impossible to create a symbol resolver for module %s"|m_filename);
    }
    return m_symbols->resolve((u64)address.pointer()-m_baseAddress, result);
}

//---------------------------------------------------------------------------//

Symbols::Symbols()
{
}

Symbols::Symbols(_TargetSelf /*self*/)
:   m_modules(Module::enumerate())
{
}

Symbols::~Symbols()
{
}

void Symbols::resolve(const Callstack::Address& address, Symbol& result) const
{
    for (size_t i = 0; i < m_modules.size(); ++i)
    {
        if(m_modules[i].resolve(address, result))
        {
            return;
        }
    }
    strncpy(result.m_module, "???", sizeof(result.m_module));
    strncpy(result.m_filename, "???", sizeof(result.m_filename));
    strncpy(result.m_function, "???", sizeof(result.m_function));
}

const Symbols& Symbols::runningSymbols()
{
    static Symbols s(Self);
    return s;
}

}}


