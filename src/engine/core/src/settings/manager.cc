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
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

#include    <core/stdafx.h>
#include    <core/settings/manager.hh>
#include    <core/environment.hh>

extern FILE* be_settings_in;
extern int be_settings_parse(void* param);

namespace BugEngine
{

SettingsManager* SettingsManager::getManager()
{
    static SettingsManager s_manager;
    return &s_manager;
}

SettingsManager::SettingsManager()
{
    be_settings_in = fopen("engine.cfg", "r");
    if(be_settings_in)
    {
        be_settings_parse(this);
    }
    ifilename p = Environment::getEnvironment().getHomeDirectory() + ifilename("engine.cfg");
    be_settings_in = fopen(p.str().c_str(), "r");
    if(be_settings_in)
    {
        be_settings_parse(this);
    }
    p = Environment::getEnvironment().getHomeDirectory() + ipath(Environment::getEnvironment().getGame()) + ifilename("engine.cfg");
    be_settings_in = fopen(p.str().c_str(), "r");
    if(be_settings_in)
    {
        be_settings_parse(this);
    }
}

SettingsManager::~SettingsManager()
{
}

}
