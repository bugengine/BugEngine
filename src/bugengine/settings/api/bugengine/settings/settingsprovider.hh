/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SETTINGS_SETTINGSPROVIDER_HH_
#define BE_SETTINGS_SETTINGSPROVIDER_HH_
/**************************************************************************************************/
#include <bugengine/settings/stdafx.h>
#include <bugengine/minitl/tuple.hh>
#include <bugengine/minitl/vector.hh>
#include <bugengine/rtti-parse/valueparse.hh>

namespace BugEngine { namespace Settings {

struct SettingsBase;
template < typename T >
struct Settings;

class be_api(SETTINGS) SettingsProvider
    : public minitl::refcountable
    , public minitl::intrusive_list< SettingsProvider >::item
{
    friend struct SettingsBase;
    template < typename T >
    friend struct Settings;

private:
    struct be_api(SETTINGS) SettingsRegistration
    {
        static minitl::intrusive_list< SettingsProvider >& getSettingsList();
        SettingsRegistration(SettingsBase & settings);
        ~SettingsRegistration();
    };
    friend struct SettingsRegistration;

protected:
    typedef minitl::vector<
        minitl::tuple< istring, ref< RTTI::AST::Namespace >, ref< RTTI::AST::Node > > >
                                                     SettingsList;
    typedef minitl::hashmap< istring, SettingsList > SettingsCategoryMap;

private:
    ifilename           m_filename;
    SettingsCategoryMap m_settings;
    ref< Folder >       m_folder;

protected:
    static void addSetting(SettingsCategoryMap & container, istring category, istring name,
                           ref< RTTI::AST::Node > value);

protected:
    SettingsProvider(const ifilename& settingsOrigin, const SettingsCategoryMap& initialSettings,
                     ref< Folder > folder);
    virtual ~SettingsProvider();

private:
    void apply(SettingsBase & settings) const;
};

}}  // namespace BugEngine::Settings

/**************************************************************************************************/
#endif
