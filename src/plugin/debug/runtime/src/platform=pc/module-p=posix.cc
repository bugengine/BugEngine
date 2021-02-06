/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.debug.runtime/stdafx.h>
#include <bugengine/plugin.debug.runtime/module.hh>
#include <bugengine/plugin.debug.runtime/symbols.hh>

#if defined(__GNU_SOURCE)
#    include <dlfcn.h>
#    include <link.h>
#endif
#include <elf.hh>
#include <pe.hh>

namespace BugEngine { namespace Runtime {

ref< const Module > Module::self()
{
    static ref< Module > s_module;
#if defined(__GNU_SOURCE)
    ref< Module > module;
    void*         handle = dlopen(0, RTLD_LAZY | RTLD_NOLOAD);
    link_map*     lmap;
    dlinfo(handle, 0, &lmap);
    for(int i = 0; lmap; lmap = lmap->l_next, i++)
    {
        if(i == 0)
        {
            /* main executable */
            /* filename seems broken */
            FILE* cmdline = fopen("/proc/self/cmdline", "r");
            if(!cmdline) continue;
            char filename[4096];
            fread(filename, 1, 4096, cmdline);
            fclose(cmdline);
            s_module = ref< Elf >::create(Arena::debug(), filename, lmap->l_addr);
            module   = s_module;
        }
        else
        {
            ref< Module > newModule
                = ref< Elf >::create(Arena::debug(), lmap->l_name, lmap->l_addr);
            module->m_next = newModule;
            module         = newModule;
        }
    }
    dlclose(handle);
#endif
    return s_module;
}

}}  // namespace BugEngine::Runtime
