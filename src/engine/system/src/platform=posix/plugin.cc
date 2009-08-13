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
#include    <dlfcn.h>

namespace BugEngine
{

Plugin::Plugin(const istring &pluginName)
{
    minitl::format<> f = (minitl::format<>("%s.so") | pluginName.c_str());
    FPluginHandle = dlopen((Environment::getEnvironment().getPluginDirectory() + ifilename(f.c_str())).str().c_str(), RTLD_NOW);
    if(! FPluginHandle)
    {
        const char* error = dlerror();
        fprintf(stderr, "%s\n", error);
        throw std::runtime_error(std::string("failed to load plugin ") + (Environment::getEnvironment().getPluginDirectory() + ifilename(f.c_str())).str().c_str() + ": " + error);
    }
    void (*_init)(void) = reinterpret_cast<void (*)(void)>(reinterpret_cast<size_t>(dlsym(FPluginHandle, "_initplugin")));
    Assert(_init);
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

