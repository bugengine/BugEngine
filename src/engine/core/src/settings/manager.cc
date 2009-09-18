/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

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
