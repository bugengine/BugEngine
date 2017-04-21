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
struct BE_EXPORT be_typeid< Settings::Settings<T> >
{
    static RTTI::Value callGet(RTTI::Value* params, u32 paramCount)
    {
        be_assert(paramCount == 0, "expected no parameter; received %d" | paramCount);
        be_forceuse(params);
        return RTTI::Value(RTTI::Value::ByRef(Settings::Settings<T>::get()));
    }
    static inline RTTI::Type  type()
    {
        return RTTI::Type::makeType(preklass(), RTTI::Type::Value, RTTI::Type::Mutable, RTTI::Type::Mutable);
    }
    static istring name()
    {
        static istring s_name(minitl::format<1024u>("Settings<%s>") | be_typeid<T>::name());
        return s_name;
    }

    static inline raw<RTTI::Class> preklass()
    {
        static RTTI::Class s_class =
        {
            name(),
            be_game_Namespace_BugEngine_Settings(),
            be_typeid<void>::klass(),
            0,
            0,
            RTTI::ClassType_Struct,
            {0},
            {0},
            {0},
            {0},
            {0},
            {0},
            0,
            0
        };
        raw<RTTI::Class> result = { &s_class };
        return result;
    }
    static inline raw<const RTTI::Class> klass()
    {
        static raw<RTTI::Class> result = preklass();
        static RTTI::Method::Overload get_overload = {
            {0},
            {0},
            be_typeid<T&>::type(),
            0,
            {0},
            true,
            &callGet
        };
        static RTTI::Method get = {
            "get",
            {0},
            {&get},
            1,
            {&get_overload}
        };
        static RTTI::ObjectInfo valueGet = {
            {0},
            {0},
            get.name,
            RTTI::Value(get)
        };
        static const RTTI::ObjectInfo* objects = result->objects.set(&valueGet);
        be_forceuse(objects);
        static RTTI::ObjectInfo s_object = {
            ::BugEngine::be_game_Namespace_BugEngine_Settings()->objects,
            {0},
            name(),
            RTTI::Value(result)
        };
        static raw<const RTTI::ObjectInfo> s_object_ptr = {&s_object};
        static const RTTI::ObjectInfo* s_object_set =
                be_game_Namespace_BugEngine_Settings()->objects.set(s_object_ptr.operator->());;
        be_forceuse(s_object_set);
        return result;
    }

};

}

/**************************************************************************************************/
#endif
