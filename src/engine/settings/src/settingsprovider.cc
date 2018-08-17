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

SettingsProvider::SettingsProvider(const ifilename& settingsOrigin,
                                   const minitl::hashmap< istring, SettingsList >& initialSettings,
                                   ref<Folder> folder)
    :   m_filename(settingsOrigin)
    ,   m_settings(Arena::general(), initialSettings)
    ,   m_folder(folder)
{
    SettingsBase::onProviderAdded(this);
}

SettingsProvider::~SettingsProvider()
{
}

SettingsProvider::SettingsRegistration::SettingsRegistration(SettingsBase& settings)
{
    const minitl::intrusive_list<SettingsProvider>& providers = getSettingsList();
    for (minitl::intrusive_list<SettingsProvider>::const_iterator it = providers.begin();
        it != providers.end();
        ++it)
    {
        it->apply(settings);
    }
}

SettingsProvider::SettingsRegistration::~SettingsRegistration()
{
}

minitl::intrusive_list<SettingsProvider>& SettingsProvider::SettingsRegistration::getSettingsList()
{
    static minitl::intrusive_list<SettingsProvider> s_providerList;
    return s_providerList;
}

void SettingsProvider::apply(SettingsBase& settings) const
{
    RTTI::Type type = RTTI::Type::makeType(settings.m_settingsClass, RTTI::Type::Value,
                                           RTTI::Type::Mutable, RTTI::Type::Mutable);
    RTTI::Value settingsValue = RTTI::Value(type , &settings);
    for (SettingsCategoryMap::const_iterator it = m_settings.begin(); it != m_settings.end(); ++it)
    {
        if (it->first == settings.m_settingsClass->name)
        {
            for (SettingsList::const_iterator setting = it->second.begin();
                 setting != it->second.end();
                 ++setting)
            {
                raw<const RTTI::Property> property = settings.m_settingsClass->getProperty(setting->first);
                if (!property)
                {
                    be_error("Unknwon setting %s in category %s" | setting->first | it->first);
                }
                else
                {
                    RTTI::Parser::DbContext context(Arena::stack(), m_filename, m_folder);
                    setting->second->resolve(context);
                    RTTI::Value v = setting->second->eval(context, property->type);
                    if (!context.errorCount)
                    {
                        property->set(settingsValue, v);
                    }
                    else
                    {
                        for (RTTI::Parser::MessageList::const_iterator message = context.messages.begin();
                             message != context.messages.end();
                             ++message)
                        {
                            Logger::root()->log(message->severity, m_filename.str(),
                                                message->location.line, message->message);
                        }
                    }
                }
            }
        }
    }
}

}}
