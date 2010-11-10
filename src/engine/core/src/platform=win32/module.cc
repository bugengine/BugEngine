/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>

#include    <core/runtime/module.hh>
#include    <core/runtime/symbols.hh>

#include    <psapi.h>

#include    <modules/elf.hh>
#include    <modules/pe.hh>


namespace BugEngine { namespace Runtime
{

ref<const Module> Module::self()
{
    static ref<Module> s_module;
    ref<Module> module;
    HANDLE process = ::GetCurrentProcess();
    DWORD requiredSize;
    ::EnumProcessModules(process, 0, 0, &requiredSize);
    size_t moduleCount = requiredSize/sizeof(HMODULE);
    Memory<Arena::TemporaryData>::Block<HMODULE> hmodules(moduleCount);
    ::EnumProcessModules(process, hmodules, requiredSize, &requiredSize);

    for(size_t i = 0; i < requiredSize/sizeof(HMODULE); i++)
    {
        char moduleName[32768];
        MODULEINFO info;
        ::GetModuleFileNameEx(process, hmodules[i], moduleName, sizeof(moduleName));
        ::GetModuleInformation(process, hmodules[i], &info, sizeof(info));
        if(i == 0)
        {
            s_module = ref<PE>::create<Arena::DebugData>(moduleName, (u64)info.lpBaseOfDll);
            module = s_module;
        }
        else
        {
            ref<Module> newModule = ref<PE>::create<Arena::DebugData>(moduleName, (u64)info.lpBaseOfDll);
            module->m_next = newModule;
            module = newModule;
        }
    }
    return s_module;
}

}}
