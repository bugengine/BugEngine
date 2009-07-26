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
#include    <core/log/logger.hh>

namespace BugEngine
{

Environment::Environment()
:   m_homeDirectory(getenv("HOME"))
,   m_rootDirectory("")
,   m_game("")
,   m_user(getenv("USER"))
{
    FILE* cmdline = fopen("/proc/self/cmdline", "r");
    if(!cmdline)
        return;
    char exe[4096];
    fread(exe, 1, 4096, cmdline);
    char* filename = exe;
    while(*filename != 0)
    {
        filename++;
    }
    while(*filename != '/' && filename != exe)
    {
        filename--;
    }
    m_game = filename;
    m_rootDirectory = (minitl::format<>("../share/%s") | filename).c_str();
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

const istring& Environment::getUser() const
{
    return m_user;
}
size_t Environment::getProcessorCount() const
{
    FILE* cpuinfo = fopen("/proc/cpuinfo", "r");
    if(!cpuinfo)
    {
        be_warning("cpuinfo not available; assuming 1 cpu");
        return 1;
    }
    int i = 0;
    while(!feof(cpuinfo))
    {
        char line[256];
        fgets(line, 256, cpuinfo);
        if(strncmp(line, "processor", 9) == 0)
        {
            sscanf(line, "processor : %d", &i);
        }
    }
    return i+1;
}

}
