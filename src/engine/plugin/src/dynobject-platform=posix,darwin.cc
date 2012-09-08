/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <plugin/plugin.hh>
#include    <core/environment.hh>

#include    <dlfcn.h>

#ifndef     PLUGIN_PREFIX
# define    PLUGIN_PREFIX "lib"
#endif
#ifndef     PLUGIN_EXT
# define    PLUGIN_EXT ".so"
#endif

namespace BugEngine
{

DynamicObject::Handle DynamicObject::load(const inamespace &pluginName, const ipath& pluginPath)
{
    const minitl::format<1024u>& plugingFile = minitl::format<1024u>(PLUGIN_PREFIX "%s" PLUGIN_EXT) | pluginName;
    const ipath& pluginDir = Environment::getEnvironment().getDataDirectory();
    ifilename::Filename pluginPath = (pluginDir + pluginPath + ifilename(plugingFile.c_str())).str();
    be_info("loading plugin %s (%s)" | pluginName | pluginPath.name);
    void* handle = dlopen(pluginPath.name, RTLD_NOW|RTLD_LOCAL);
    if (!handle)
    {
        be_error(dlerror());
    }
    return handle;
}

void DynamicObject::unload(Handle handle)
{
    dlclose(handle);
}

void* DynamicObject::getSymbolInternal(Handle handle, const char* name)
{
    return dlsym(handle, name);
}


}
