/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <settings/stdafx.h>
#include    <settings/settingsprovider.hh>
#include    <settings/settings.factory.hh>
#include    <rtti/engine/namespace.hh>

namespace BugEngine { namespace Settings
{

void SettingsProvider::addSetting(minitl::hashmap<istring, SettingsList> &container,
                                  istring category,
                                  istring name,
                                  ref<RTTI::Parser::Node> value)
{
    minitl::hashmap<istring, SettingsList>::iterator where;
    where = container.insert(category, SettingsList(Arena::general())).first;
    for (SettingsList::iterator it = where->second.begin(); it != where->second.end(); ++it)
    {
        if (it->first == name)
        {
            be_warning("setting %s.%s overriden; first value ignored" | category | name);
            it->second = value;
            return;
        }
    }
    where->second.push_back(minitl::make_tuple(name, value));
}

SettingsProvider::SettingsProvider(const minitl::hashmap< istring, SettingsList >& initialSettings)
    :   m_settings(Arena::general(), initialSettings)
{
    SettingsBase::onProviderAdded(this);
}

SettingsProvider::~SettingsProvider()
{
}

SettingsProvider::SettingsRegistration::SettingsRegistration()
{
}

SettingsProvider::SettingsRegistration::~SettingsRegistration()
{
}

minitl::intrusive_list<SettingsProvider>& SettingsProvider::SettingsRegistration::getSettingsList()
{
    static minitl::intrusive_list<SettingsProvider> s_providerList;
    return s_providerList;
}

}}
