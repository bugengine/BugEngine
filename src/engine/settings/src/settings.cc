/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <settings/stdafx.h>
#include    <settings/settings.factory.hh>
#include    <rtti/engine/namespace.hh>

BE_REGISTER_NAMESPACE_2_NAMED(bugengine, BugEngine, Settings);

namespace BugEngine { namespace Settings
{

minitl::intrusive_list<SettingsBase>& SettingsBase::getSettings()
{
    static minitl::intrusive_list<SettingsBase> s_settingsList;
    return s_settingsList;
}

SettingsBase::SettingsBase(raw<const RTTI::Class> settingsClass)
    :   m_settingsClass(settingsClass)
{
    getSettings().push_back(*this);
}

SettingsBase::~SettingsBase()
{
    be_assert(this->hooked(), "Settings object %s was removed from the settings list"
                            | m_settingsClass->fullname());
    this->unhook();
}

void SettingsBase::onProviderAdded(weak<const SettingsProvider> provider)
{
    for (minitl::intrusive_list<SettingsBase>::const_iterator it = getSettings().begin();
         it != getSettings().end();
         ++it)
    {
        SettingsProvider::SettingsCategoryMap::const_iterator settingsOverride;
        settingsOverride = provider->m_settings.find(it->m_settingsClass->name);
        if (settingsOverride != provider->m_settings.end())
        {
            for (SettingsProvider::SettingsList::const_iterator s = settingsOverride->second.begin();
                 s != settingsOverride->second.end();
                 ++s)
            {
                be_info("override %s.%s" | settingsOverride->first | s->first);
            }
        }
    }
}




}}
