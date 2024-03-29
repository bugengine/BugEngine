/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.debug.runtime/stdafx.h>
#include <bugengine/plugin.debug.runtime/module.hh>
#include <bugengine/plugin.debug.runtime/symbols.hh>

#include <psapi.h>

#include <elf.hh>
#include <pe.hh>

namespace BugEngine { namespace Runtime {

ref< const Module > Module::self()
{
    static ref< Module > s_module;
    static ref< Module > module;
    static size_t        seen = 0;

    HANDLE process = ::GetCurrentProcess();
    DWORD  requiredSize;
    ::EnumProcessModules(process, 0, 0, &requiredSize);
    size_t                              moduleCount = requiredSize / sizeof(HMODULE);
    minitl::Allocator::Block< HMODULE > hmodules(Arena::stack(), moduleCount);
    ::EnumProcessModules(process, hmodules, requiredSize, &requiredSize);

    for(; seen < moduleCount; seen++)
    {
        char       moduleName[32768];
        MODULEINFO info;
        ::GetModuleFileNameEx(process, hmodules[seen], moduleName, sizeof(moduleName));
        ::GetModuleInformation(process, hmodules[seen], &info, sizeof(info));
        if(seen == 0)
        {
            s_module
                = ref< PE >::create(Arena::debug(), moduleName, (u64)(uintptr_t)info.lpBaseOfDll);
            module = s_module;
        }
        else
        {
            ref< Module > newModule
                = ref< PE >::create(Arena::debug(), moduleName, (u64)(uintptr_t)info.lpBaseOfDll);
            module->m_next = newModule;
            module         = newModule;
        }
    }
    return s_module;
}

}}  // namespace BugEngine::Runtime
