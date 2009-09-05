/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA                                                                  *
* 02110-1301  USA                                                             *
\*****************************************************************************/
    
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

