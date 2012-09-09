/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <plugin/stdafx.h>
#include    <plugin/dynobject.hh>
#include    <core/environment.hh>

#include    <dlfcn.h>

#ifndef     PLUGIN_PREFIX
# define    PLUGIN_PREFIX "lib"
#endif
#ifndef     PLUGIN_EXT
# define    PLUGIN_EXT ".so"
#endif

namespace BugEngine { namespace Plugin
{

DynamicObject::Handle DynamicObject::load(const inamespace &pluginName, const ipath& pluginPath)
{
    const minitl::format<1024u>& plugingFile = minitl::format<1024u>(PLUGIN_PREFIX "%s" PLUGIN_EXT) | pluginName;
    const ipath& pluginDir = Environment::getEnvironment().getDataDirectory();
    ifilename::Filename fullPath = (pluginDir + pluginPath + ifilename(plugingFile.c_str())).str();
    be_info("loading plugin %s (%s)" | pluginName | fullPath.name);
    void* handle = dlopen(fullPath.name, RTLD_NOW|RTLD_LOCAL);
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

void* DynamicObject::getSymbolInternal(Handle handle, const istring& name)
{
    return dlsym(handle, name.c_str());
}

}}
