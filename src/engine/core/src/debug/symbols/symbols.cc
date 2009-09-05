/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

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
    m_filename[0] = 0;
    m_function[0] = 0;
}

Symbols::Symbol::~Symbol()
{
}

//---------------------------------------------------------------------------//

Symbols::Module::Module(const char *filename, u64 /*baseAddress*/)
{
    FILE* f = fopen(filename, "rb");
    if(f)
    {
        char signature[2];
        fread(signature, 1, 2, f);
        fseek(f, 0, SEEK_SET);
        if (signature[0] == 'M' && signature[1] == 'Z')
        {
            PE pe(filename, f);
        }
        else if (signature[0] == 0x7f && signature[1] == 'E')
        {
            Elf e(filename, f);
        }
        fclose(f);
    }
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
        if(i == 0)
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
    HMODULE* hmodules = (HMODULE*)be_malloc(requiredSize);
    ::EnumProcessModules(process, hmodules, requiredSize, &requiredSize);
    for(size_t i = 0; i < requiredSize/sizeof(HMODULE); i++)
    {
        char moduleName[32768];
        MODULEINFO info;
        ::GetModuleFileNameEx(process, hmodules[i], moduleName, sizeof(moduleName));
        ::GetModuleInformation(process, hmodules[i], &info, sizeof(info));
        modules.push_back(Module(moduleName, (u64)info.lpBaseOfDll));
    }
    be_free(hmodules);
#else
# error platform not supported yet...
#endif
    return modules;
}

bool Symbols::Module::resolve(const Callstack::Address& /*address*/, Symbol& /*result*/) const
{
    return false;
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
    strcpy(result.m_filename, "???");
    strcpy(result.m_function, "???");
}

const Symbols& Symbols::runningSymbols()
{
    static Symbols s(Self);
    return s;
}

}}


