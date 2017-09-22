/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SETTINGS_SETTINGS_FACTORY_HH_
#define BE_SETTINGS_SETTINGS_FACTORY_HH_
/**************************************************************************************************/
#include    <settings/stdafx.h>
#include    <settings/settingsprovider.hh>
#include    <rtti/typeinfo.hh>
#include    <rtti/engine/propertyinfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/engine/objectinfo.script.hh>
#include    <rtti/value.hh>


namespace BugEngine { namespace Settings
{

template< typename T >
struct Settings;

struct be_api(Settings) SettingsBase : public minitl::intrusive_list<SettingsBase>::item
{
    template< typename T >
    friend struct Settings;
    friend class SettingsProvider;
private:
    static minitl::intrusive_list<SettingsBase>& getSettings();
private:
    raw<const RTTI::Class> const    m_settingsClass;
private:
    static void onProviderAdded(weak<const SettingsProvider> provider);
protected:
    SettingsBase(raw<const RTTI::Class> settingsClass);
    ~SettingsBase();
};


template< typename T >
struct Settings : public SettingsBase
{
private:
    static BE_EXPORT T                                      s_settings;
    static BE_EXPORT SettingsProvider::SettingsRegistration s_registration;
protected:
    Settings()
        :   SettingsBase(be_typeid<T>::klass())
    {
        (void)s_registration;
    }

public:
    static BE_EXPORT const T& get() { return s_settings; }
};

template< typename T >
T Settings<T>::s_settings;
template< typename T >
SettingsProvider::SettingsRegistration Settings<T>::s_registration;

}}

namespace BugEngine
{

raw<RTTI::Class> be_game_Namespace_BugEngine_Settings();

template< typename T >
struct be_typeid< Settings::Settings<T> >
{
    static BE_EXPORT raw<const RTTI::Class> klass();
    static BE_EXPORT RTTI::Type  type();
};


namespace Settings
{

template< typename T >
struct BE_EXPORT Settings_BugHelper
{
    static RTTI::Value trampoline_method_get_overload_0(RTTI::Value* parameters, u32 parameterCount);
    static const RTTI::Method::Overload s_method_get_overloads[];
    static const RTTI::Method s_methods[];
};

template< typename T >
RTTI::Value Settings_BugHelper<T>::trampoline_method_get_overload_0(RTTI::Value* params, u32 paramCount)
{
    be_assert(paramCount == 0, "expected no parameter; received %d" | paramCount);
    be_forceuse(params);
    return RTTI::Value(RTTI::Value::ByRef(Settings<T>::get()));
}

template< typename T >
const RTTI::Method::Overload Settings_BugHelper<T>::s_method_get_overloads[] = {
    {
        { 0 },
        { 0, 0 },
        be_typeid< T& >::type(),
        false,
        &trampoline_method_get_overload_0
    }
};

template< typename T >
const RTTI::Method Settings_BugHelper<T>::s_methods[1] = {
    {
        istring("get"),
        { 1, s_method_get_overloads },
        {&s_methods[0]}
    }
};

}


template< typename T >
BE_EXPORT
raw<const RTTI::Class> be_typeid< Settings::Settings<T> >::klass()
{
    static const RTTI::Class s_class =
    {
        istring(minitl::format<1024u>("Settings<%s>") | be_typeid<T>::name()),
        0,
        0,
        RTTI::ClassType_Struct,
        {be_game_Namespace_BugEngine_Settings().m_ptr},
        be_typeid<void>::klass(),
        {0},
        {0},
        {0, 0},
        {1, Settings::Settings_BugHelper<T>::s_methods},
        {0},
        {0},
        0,
        0
    };

    raw< const RTTI::Class > result = { &s_class };
    return result;
}

template< typename T >
BE_EXPORT
RTTI::Type be_typeid< Settings::Settings<T> >::type()
{
    return RTTI::Type::makeType(klass(), RTTI::Type::Value, RTTI::Type::Mutable, RTTI::Type::Mutable);
}


}

/**************************************************************************************************/
#endif
