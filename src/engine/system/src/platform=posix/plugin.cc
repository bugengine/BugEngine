/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/plugin.hh>
#include    <core/environment.hh>

#include    <stdexcept>
#include    <dlfcn.h>

namespace BugEngine
{

Plugin::Plugin(const istring &pluginName)
{
    minitl::format<> f = (minitl::format<>("lib%s.so") | pluginName.c_str());
    FPluginHandle = dlopen((Environment::getEnvironment().getPluginDirectory() + ifilename(f.c_str())).str().c_str(), RTLD_NOW);
    if(! FPluginHandle)
    {
        const char* error = dlerror();
        fprintf(stderr, "%s\n", error);
        throw std::runtime_error(std::string("failed to load plugin ") + (Environment::getEnvironment().getPluginDirectory() + ifilename(f.c_str())).str().c_str() + ": " + error);
    }
    void (*_init)(void) = reinterpret_cast<void (*)(void)>(reinterpret_cast<size_t>(dlsym(FPluginHandle, "_initplugin")));
    be_assert(_init, "could not find method _init in plugin %s" | pluginName.c_str());
    (*_init)();
}

Plugin::~Plugin(void)
{
    void (*_fini)(void) = reinterpret_cast<void (*)(void)>(reinterpret_cast<size_t>(dlsym(FPluginHandle, "_finiplugin")));
    if(_fini)
        _fini(); 
    dlclose(FPluginHandle);
}

Plugin::generic Plugin::_get(const std::string& symbol)
{
    return reinterpret_cast<generic>(reinterpret_cast<size_t>(dlsym(FPluginHandle,symbol.c_str())));
}

}

