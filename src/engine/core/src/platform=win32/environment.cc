/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/environment.hh>
#include    <userenv.h>

namespace BugEngine
{

Environment::Environment()
:   m_homeDirectory("")
,   m_dataDirectory("")
,   m_pluginDirectory("")
,   m_game("")
,   m_user("")
{
    HANDLE token;
    OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token);
    char profile[MAX_PATH];
    DWORD size = sizeof(profile);
    GetUserName(profile, &size);
    m_user = profile;
    size = sizeof(profile);
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
    m_dataDirectory = ipath(exe, exe+begin);
    m_dataDirectory.push_back(istring("data"));
    m_pluginDirectory = m_dataDirectory;
    m_pluginDirectory.push_back(istring("plugins"));
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

const ipath& Environment::getDataDirectory() const
{
    return m_dataDirectory;
}

const ipath& Environment::getPluginDirectory() const
{
    return m_pluginDirectory;
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
    SYSTEM_INFO i;
    GetSystemInfo(&i);
    return i.dwNumberOfProcessors;
}

}
