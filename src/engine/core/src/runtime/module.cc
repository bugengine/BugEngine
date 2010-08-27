/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>

#include    <core/runtime/module.hh>
#include    <core/runtime/symbols.hh>

#if defined(BE_PLATFORM_POSIX) && defined(__GNU_SOURCE)
# include   <dlfcn.h>
# include   <link.h>
#endif
#ifdef BE_PLATFORM_WIN32
# include   <psapi.h>
#endif

#include    <modules/elf.hh>
#include    <modules/pe.hh>


namespace BugEngine { namespace Runtime
{

Module::Module(const char *filename, u64 baseAddress)
:   m_filename(filename)
,   m_baseAddress(baseAddress)
{
}

Module::~Module()
{
}

#ifdef BE_PLATFORM_PC
ref<const Module> Module::self()
{
    static ref<Module> s_module;
    ref<Module> module;
#if defined(BE_PLATFORM_POSIX) && defined(__GNU_SOURCE)
    void* handle = dlopen(0, RTLD_LAZY|RTLD_NOLOAD);
    link_map* lmap;
    dlinfo(handle, 0, &lmap);
    for(int i = 0; lmap; lmap=lmap->l_next, i++)
    {
        if(i == 0)
        {
            /* main executable */
            /* filename seems broken */
            FILE* cmdline = fopen("/proc/self/cmdline", "r");
            if(!cmdline)
                continue;
            char filename[4096];
            fread(filename, 1, 4096, cmdline);
            s_module = ref<Elf>::create<Arena::General>(filename, lmap->l_addr);
            module = s_module;
        }
        else
        {
            ref<Module> newModule = ref<Elf>::create<Arena::General>(lmap->l_name, lmap->l_addr);
            module->m_next = newModule;
            module = newModule;
        }
    }
    dlclose(handle);
#elif defined(BE_PLATFORM_WIN32)
    HANDLE process = ::GetCurrentProcess();
    DWORD requiredSize;
    ::EnumProcessModules(process, 0, 0, &requiredSize);
    size_t moduleCount = requiredSize/sizeof(HMODULE);
    Memory<Arena::General>::Block<HMODULE> hmodules(moduleCount);
    ::EnumProcessModules(process, hmodules, requiredSize, &requiredSize);

    for(size_t i = 0; i < requiredSize/sizeof(HMODULE); i++)
    {
        char moduleName[32768];
        MODULEINFO info;
        ::GetModuleFileNameEx(process, hmodules[i], moduleName, sizeof(moduleName));
        ::GetModuleInformation(process, hmodules[i], &info, sizeof(info));
        if(i == 0)
        {
           s_module = ref<PE>::create<Arena::General>(moduleName, (u64)info.lpBaseOfDll);
            module = s_module;
        }
        else
        {
            ref<Module> newModule = ref<PE>::create<Arena::General>(moduleName, (u64)info.lpBaseOfDll);
            module->m_next = newModule;
            module = newModule;
        }
    }
#endif
    return s_module;
}
#endif

const Module::Section& Module::operator[](const istring& name) const
{
    static Section s_empty = { "", 0, 0, 0, 0 };
    for(minitl::vector<Section, Arena::General>::const_iterator it = m_sections.begin(); it != m_sections.end(); ++it)
    {
        if(it->name == name)
            return *it;
    }
    return s_empty;
}

void Module::readSection(const Section &section, void *data) const
{
    FILE*f = fopen(m_filename.str().c_str(), "rb");
    fseek(f, be_checked_numcast<long int>(section.fileOffset), SEEK_SET);
    fread(data, be_checked_numcast<long int>(section.fileSize), 1, f);
}


}}


