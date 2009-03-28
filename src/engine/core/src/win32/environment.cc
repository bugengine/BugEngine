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
#include    <core/environment.hh>
#include    <userenv.h>

namespace BugEngine
{

Environment::Environment()
:   m_homeDirectory("")
,   m_rootDirectory("")
,   m_game("")
{
    HANDLE token;
    OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token);
    char profile[MAX_PATH];
    DWORD size = sizeof(profile);
    GetUserProfileDirectory(token, profile, &size);
    m_homeDirectory = profile;
    m_homeDirectory += "BugEngine";

    const char *exe = __argv[0];
    size_t s = strlen(__argv[0])-1;
    do
    {
        if(exe[s] == '\\' || exe[s] == '/')
            break;
        s--;
    }
    while(s != 0);
    size_t begin = s;
    while(exe[s] && exe[s] != '.')
        s++;
    m_rootDirectory = ipath(exe, exe+begin);
    m_game = istring(exe+begin+1, exe+s);

}

Environment::~Environment()
{
}

const Environment& Environment::getEnvironment()
{
    static Environment s_environment;
    return s_environment;
}

const ipath& Environment::getRootDirectory() const
{
    return m_rootDirectory;
}

const ipath& Environment::getHomeDirectory() const
{
    return m_homeDirectory;
}

const istring& Environment::getGame() const
{
    return m_game;
}

size_t Environment::getProcessorCount() const
{
    SYSTEM_INFO i;
    GetSystemInfo(&i);
    return i.dwNumberOfProcessors;
}

}
