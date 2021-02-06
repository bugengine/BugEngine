/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin/stdafx.h>
#include <bugengine/core/environment.hh>
#include <bugengine/plugin/dynobject.hh>

#if !BE_STATIC

#    include <dlfcn.h>

#    ifndef PLUGIN_PREFIX
#        define PLUGIN_PREFIX "lib"
#    endif
#    ifndef PLUGIN_EXT
#        define PLUGIN_EXT ".so"
#    endif

#    define PLUGIN_FILE_NAME PLUGIN_PREFIX "%s" PLUGIN_EXT

namespace BugEngine { namespace Plugin {

DynamicObject::Handle DynamicObject::load(const inamespace& pluginName, const ipath& pluginPath)
{
    be_forceuse(pluginPath);
    const minitl::format< 1024u > pluginFile
        = minitl::format< 1024u >(PLUGIN_FILE_NAME) | pluginName;
    be_info("loading dynamic object %s" | pluginName);
    void* handle = dlopen(pluginFile, RTLD_NOW | RTLD_LOCAL);
    if(!handle)
    {
        be_error("Error loading dynamic object %s: %s" | pluginName | dlerror());
    }
    return handle;
}

void DynamicObject::unload(Handle handle)
{
    dlclose(handle);
}

void* DynamicObject::getSymbolInternal(Handle handle, const istring& name)
{
    return dlsym(handle, name.c_str());
}

}}  // namespace BugEngine::Plugin

#endif
