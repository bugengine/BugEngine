/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/settings/stdafx.h>
#include <bugengine/rtti/engine/namespace.hh>
#include <bugengine/settings/settings.factory.hh>
#include <bugengine/settings/settingsprovider.hh>

namespace BugEngine { namespace Settings {

void SettingsProvider::addSetting(minitl::hashmap< istring, SettingsList >& container,
                                  istring category, istring name, ref< RTTI::AST::Node > value)
{
    minitl::hashmap< istring, SettingsList >::iterator where;
    where = container.insert(category, SettingsList(Arena::general())).first;
    for(SettingsList::iterator it = where->second.begin(); it != where->second.end(); ++it)
    {
        if(it->first == name)
        {
            be_warning("setting %s.%s overriden; first value ignored" | category | name);
            it->third = value;
            return;
        }
    }
    where->second.push_back(minitl::make_tuple(
        name, ref< RTTI::AST::Namespace >::create(Arena::general(), byref(Arena::general())),
        value));
}

SettingsProvider::SettingsProvider(const minitl::hashmap< istring, SettingsList >& initialSettings,
                                   ref< Folder >                                   folder)
    : m_settings(Arena::general(), initialSettings)
    , m_folder(folder)
{
    SettingsRegistration::getSettingsList().push_back(*this);
    SettingsBase::onProviderAdded(this);
}

SettingsProvider::~SettingsProvider()
{
    this->unhook();
}

SettingsProvider::SettingsRegistration::SettingsRegistration(SettingsBase& settings)
{
    const minitl::intrusive_list< SettingsProvider >& providers = getSettingsList();
    for(minitl::intrusive_list< SettingsProvider >::const_iterator it = providers.begin();
        it != providers.end(); ++it)
    {
        it->apply(settings);
    }
}

SettingsProvider::SettingsRegistration::~SettingsRegistration()
{
}

minitl::intrusive_list< SettingsProvider >&
SettingsProvider::SettingsRegistration::getSettingsList()
{
    static minitl::intrusive_list< SettingsProvider > s_providerList;
    return s_providerList;
}

void SettingsProvider::apply(SettingsBase& settings) const
{
    RTTI::Type  type          = RTTI::Type::makeType(settings.m_settingsClass, RTTI::Type::Value,
                                           RTTI::Type::Mutable, RTTI::Type::Mutable);
    RTTI::Value settingsValue = RTTI::Value(type, &settings);
    for(SettingsCategoryMap::const_iterator it = m_settings.begin(); it != m_settings.end(); ++it)
    {
        if(it->first == settings.m_settingsClass->name)
        {
            for(SettingsList::const_iterator setting = it->second.begin();
                setting != it->second.end(); ++setting)
            {
                raw< const RTTI::Property > property
                    = settings.m_settingsClass->getProperty(setting->first);
                if(!property)
                {
                    be_error("Unknwon setting %s in category %s" | setting->first | it->first);
                }
                else
                {
                    RTTI::AST::DbContext context(Arena::stack(), setting->second, m_folder);
                    setting->third->resolve(context);
                    RTTI::Value v = setting->third->eval(property->type);
                    if(!context.errorCount)
                    {
                        property->set(settingsValue, v);
                    }
                    for(RTTI::AST::MessageList::const_iterator message = context.messages.begin();
                        message != context.messages.end(); ++message)
                    {
                        log(*message);
                    }
                }
            }
        }
    }
}

}}  // namespace BugEngine::Settings
