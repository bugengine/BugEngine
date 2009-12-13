/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdexcept>
#include    <core/environment.hh>


#define BE_PLUGIN_REGISTER(name, klass, args)                                   \
    extern "C" FORCEEXPORT klass* be_createPlugin() { return new klass args; }  \
    extern "C" FORCEEXPORT void be_destroyPlugin(klass* cls) { delete cls; }

namespace BugEngine
{

template< typename Interface >
Plugin<Interface>::Plugin(const istring &pluginName)
{
    SetLastError(0);
    std::string pluginDir = Environment::getEnvironment().getDataDirectory().str();
    m_handle = LoadLibrary( (pluginDir + "/plugins/" + pluginName.c_str() + ".dll").c_str());
    if(m_handle != 0)
    {
        Interface* (*be_pluginCreate)(void) = reinterpret_cast<Interface* (*)(void)>(GetProcAddress(static_cast<HINSTANCE>(m_handle), "be_createPlugin"));
        if (!be_pluginCreate)
        {
            char *_errorMessage;
            ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                             NULL,
                             ::GetLastError(),
                             0,
                             reinterpret_cast<LPTSTR>(&_errorMessage),
                             0,
                             NULL);
            std::string errorMessage(_errorMessage);
            ::LocalFree(_errorMessage);
            throw std::runtime_error(std::string("failed to initialize plugin ") + pluginName.c_str() + " : " + errorMessage);
        }
        else
        {
            m_interface = (*be_pluginCreate)();
        }
    }
    else
    {
        char *_errorMessage;
        ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                         NULL,
                         ::GetLastError(),
                         0,
                         reinterpret_cast<LPTSTR>(&_errorMessage),
                         0,
                         NULL);
        std::string errorMessage(_errorMessage);
        ::LocalFree(_errorMessage);
        throw std::runtime_error(std::string("failed to load plugin ") + pluginName.c_str() + " : " + errorMessage);
    }
}

template< typename Interface >
Plugin<Interface>::~Plugin(void)
{
    void (*be_pluginDestroy)(Interface*) = reinterpret_cast<void (*)(Interface*)>(GetProcAddress(static_cast<HINSTANCE>(m_handle), "be_destroyPlugin"));
    if(be_pluginDestroy)
        (*be_pluginDestroy)(m_interface); 
    FreeLibrary(static_cast<HMODULE>(m_handle));
}

}

