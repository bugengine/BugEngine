/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/settings/stdafx.h>
#include <bugengine/meta/engine/namespace.hh>
#include <bugengine/settings/settings.factory.hh>

BE_REGISTER_NAMESPACE_2_NAMED(bugengine, BugEngine, Settings);

namespace BugEngine { namespace Settings {

minitl::intrusive_list< SettingsBase >& SettingsBase::getSettings()
{
    static minitl::intrusive_list< SettingsBase > s_settingsList;
    return s_settingsList;
}

SettingsBase::SettingsBase(raw< const Meta::Class > settingsClass) : m_settingsClass(settingsClass)
{
    getSettings().push_back(*this);
}

SettingsBase::~SettingsBase()
{
    be_assert(this->hooked(), "Settings object %s was removed from the settings list"
                                  | m_settingsClass->fullname());
    this->unhook();
}

void SettingsBase::onProviderAdded(weak< const SettingsProvider > provider)
{
    for(minitl::intrusive_list< SettingsBase >::iterator it = getSettings().begin();
        it != getSettings().end(); ++it)
    {
        provider->apply(*it);
    }
}

}}  // namespace BugEngine::Settings
