/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>

#include    <core/runtime/module.hh>
#include    <core/runtime/symbols.hh>

#ifdef BE_PLATFORM_POSIX
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

refptr<const Module> Module::self()
{
    static refptr<Module> s_module;
    refptr<Module> module;
#ifdef BE_PLATFORM_POSIX
    void* handle = dlopen(0, RTLD_LAZY);
    link_map* lmap;
    dlinfo(handle, RTLD_DI_LINKMAP, &lmap);
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
            s_module = new Elf(filename, lmap->l_addr);
        }
        else
        {
            s_module->m_depends.push_back(new Elf(lmap->l_name, lmap->l_addr));
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
        if(i == 0)
        {
            s_module = new PE(moduleName, (u64)info.lpBaseOfDll);
            module = s_module;
        }
        else
        {
            refptr<Module> newModule = new PE(moduleName, (u64)info.lpBaseOfDll);
            module->m_next = newModule;
            module = newModule;
        }
    }
#else
# error platform not supported yet...
#endif
    return s_module;
}

const Module::Section& Module::operator[](const istring& name) const
{
    static Section s_empty = { "", 0, 0, 0, 0 };
    for(minitl::vector<Section>::const_iterator it = m_sections.begin(); it != m_sections.end(); ++it)
    {
        if(it->name == name)
            return *it;
    }
    return s_empty;
}

void Module::readSection(const Section &section, void *data) const
{
    FILE*f = fopen(m_filename.str().c_str(), "rb");
    fseek(f, checked_numcast<long int>(section.fileOffset), SEEK_SET);
    fread(data, checked_numcast<long int>(section.fileSize), 1, f);
}


}}


