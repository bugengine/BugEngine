/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_SETTINGS_SETTINGS_FACTORY_HH_
#define BE_SETTINGS_SETTINGS_FACTORY_HH_
/**************************************************************************************************/
#include <bugengine/settings/stdafx.h>
#include <bugengine/meta/engine/methodinfo.script.hh>
#include <bugengine/meta/engine/objectinfo.script.hh>
#include <bugengine/meta/engine/propertyinfo.script.hh>
#include <bugengine/meta/typeinfo.hh>
#include <bugengine/meta/value.hh>
#include <bugengine/settings/settingsprovider.hh>

namespace BugEngine { namespace Settings {

template < typename T >
struct Settings;

struct be_api(SETTINGS) SettingsBase : public minitl::intrusive_list< SettingsBase >::item
{
    template < typename T >
    friend struct Settings;
    friend class SettingsProvider;

private:
    static minitl::intrusive_list< SettingsBase >& getSettings();

private:
    raw< const Meta::Class > const m_settingsClass;

private:
    static void onProviderAdded(weak< const SettingsProvider > provider);

protected:
    SettingsBase(raw< const Meta::Class > settingsClass);
    ~SettingsBase();
};

template < typename T >
struct Settings : public SettingsBase
{
private:
    static BE_EXPORT SettingsProvider::SettingsRegistration s_registration;
    static BE_EXPORT T& getStaticSettings()
    {
        static T s_settings;
        return s_settings;
    }

protected:
    Settings() : SettingsBase(be_class< T >())
    {
        (void)s_registration;
    }

public:
    static BE_EXPORT const T& get()
    {
        return getStaticSettings();
    }
};

template < typename T >
BE_EXPORT SettingsProvider::SettingsRegistration Settings< T >::s_registration(getStaticSettings());

}}  // namespace BugEngine::Settings

namespace BugEngine {

BE_EXPORT raw< Meta::Class > be_bugengine_Namespace_BugEngine_Settings();

namespace Meta {
template < typename T >
struct ClassID< Settings::Settings< T > >
{
    static BE_EXPORT raw< const Meta::Class > klass();
};
}  // namespace Meta

namespace Settings {

template < typename T >
struct BE_EXPORT Settings_BugHelper
{
    static Meta::Value                  trampoline_method_get_overload_0(Meta::Value* parameters,
                                                                         u32          parameterCount);
    static const Meta::Method::Overload s_method_get_overloads[];
    static const Meta::Method           s_methods[];
};

template < typename T >
Meta::Value Settings_BugHelper< T >::trampoline_method_get_overload_0(Meta::Value* params,
                                                                      u32          paramCount)
{
    be_assert(paramCount == 0, "expected no parameter; received %d" | paramCount);
    be_forceuse(params);
    return Meta::Value(Meta::Value::ByRef(Settings< T >::get()));
}

template < typename T >
const Meta::Method::Overload Settings_BugHelper< T >::s_method_get_overloads[]
    = {{{0}, {0, 0}, be_type< T& >(), false, {0, 0}, &trampoline_method_get_overload_0}};

template < typename T >
const Meta::Method Settings_BugHelper< T >::s_methods[1]
    = {{istring("get"), {1, s_method_get_overloads}, {0}}};

}  // namespace Settings

template < typename T >
BE_EXPORT raw< const Meta::Class > Meta::ClassID< Settings::Settings< T > >::klass()
{
    static Meta::Class       s_class = {"Settings",
                                  0,
                                  0,
                                  Meta::ClassType_Struct,
                                  {be_bugengine_Namespace_BugEngine_Settings().m_ptr},
                                  be_class< void >(),
                                  {0},
                                  {0},
                                  {0, 0},
                                  {1, Settings::Settings_BugHelper< T >::s_methods},
                                  {0},
                                  {0},
                                  0,
                                  0};
    raw< const Meta::Class > result  = {&s_class};
    return result;
}

}  // namespace BugEngine

/**************************************************************************************************/
#endif
