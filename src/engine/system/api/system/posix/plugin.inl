/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/plugin.hh>
#include    <core/environment.hh>

#include    <stdexcept>
#include    <dlfcn.h>


#define BE_PLUGIN_REGISTER(name, klass, args)                                   \
    extern "C" FORCEEXPORT klass* be_createPlugin() { return new klass args; }  \
    extern "C" FORCEEXPORT void be_destroyPlugin(klass* cls) { delete cls; }

namespace BugEngine
{

template< typename Interface >
Plugin<Interface>::Plugin(const istring &pluginName)
{
    minitl::format<> f = (minitl::format<>("lib%s.so") | pluginName.c_str());
    m_handle = dlopen((Environment::getEnvironment().getDataDirectory() + ipath("plugins") + ifilename(f.c_str())).str().c_str(), RTLD_NOW);
    if(! m_handle)
    {
        const char* error = dlerror();
        fprintf(stderr, "%s\n", error);
        throw std::runtime_error(std::string("failed to load plugin ") + f.c_str() + ": " + error);
    }
    Interface* (*_init)(void) = reinterpret_cast<Interface* (*)(void)>(reinterpret_cast<size_t>(dlsym(m_handle, "be_createPlugin")));
    be_assert(_init, "could not find method _init in plugin %s" | pluginName.c_str());
    m_interface = (*_init)();
}

template< typename Interface >
Plugin<Interface>::~Plugin(void)
{
    void (*_fini)(Interface*) = reinterpret_cast<void (*)(Interface*)>(reinterpret_cast<size_t>(dlsym(m_handle, "be_destroyPlugin")));
    if(_fini)
        _fini(m_interface); 
    dlclose(m_handle);
}

}

