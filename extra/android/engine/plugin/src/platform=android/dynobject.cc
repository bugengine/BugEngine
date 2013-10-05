/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <plugin/stdafx.h>
#include    <plugin/dynobject.hh>

#if !BE_STATIC

#include    <dlfcn.h>

namespace BugEngine { namespace Plugin
{

DynamicObject::Handle DynamicObject::load(const inamespace &pluginName, const ipath& /*pluginPath*/)
{
    const minitl::format<1024u>& pluginFile = minitl::format<1024u>("lib%s.so") | pluginName;
    be_info("loading plugin %s (%s)" | pluginName | pluginFile);
    void* handle = dlopen(pluginFile, RTLD_NOW|RTLD_GLOBAL);
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

#endif
