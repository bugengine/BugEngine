/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/plugin.hh>
#include    <core/environment.hh>

#include    <stdexcept>

namespace BugEngine
{

Plugin::Plugin(const istring &pluginName)
{
    SetLastError(0);
	std::string pluginDir = Environment::getEnvironment().getPluginDirectory().str();
    FPluginHandle = LoadLibrary( (pluginDir + "/" + pluginName.c_str() + ".dll").c_str());
    if(FPluginHandle != 0)
    {
        void (CALLBACK *_init)(void) = reinterpret_cast<void (CALLBACK*)(void)>(GetProcAddress(static_cast<HINSTANCE>(FPluginHandle), "_initplugin"));
        if (!_init)
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
            be_assert(_init, "could not find method _init in plugin %s" | pluginName.c_str());
            _init();
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
/*****************************************************************************/
Plugin::~Plugin(void)
{
    void (CALLBACK *_fini)(void) = reinterpret_cast<void (CALLBACK*)(void)>(GetProcAddress(static_cast<HINSTANCE>(FPluginHandle), "_finiplugin"));
    if(_fini)
        _fini(); 
    FreeLibrary(static_cast<HMODULE>(FPluginHandle));
}

Plugin::generic Plugin::_get(const std::string& symbol)
{
    SetLastError(0);
    generic result = (generic)GetProcAddress(static_cast<HINSTANCE>(FPluginHandle),symbol.c_str());
    if(!result)
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
        throw std::runtime_error(std::string("failed to load symbol ") + symbol + " : " + errorMessage);
    }
    return result;
}

}

