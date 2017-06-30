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
    be_section(rtti_text_trampoline)
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

    be_section(rtti_text_cls)
    static inline raw<RTTI::Class> preklass()
    {
        be_section(rtti_cls)
        static RTTI::Class s_class =
        {
            name(),
            0,
            0,
            RTTI::ClassType_Struct,
            be_game_Namespace_BugEngine_Settings(),
            be_typeid<void>::klass(),
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
        static raw<const RTTI::Class> result = registerProperties();
        return result;
    }

    be_section(rtti_text_cls_props)
    static raw<const RTTI::Class> registerProperties()
    {
        static raw<RTTI::Class> result = preklass();
        be_section(rtti_method)
        static RTTI::staticarray_n< 1, const RTTI::Method::Overload > s_method_get_overloads = {
            1,
            {
                {
                    {0},
                    {0},
                    be_typeid<T&>::type(),
                    false,
                    &callGet
                },
            }
        };
        be_section(rtti_method)
        static RTTI::Method s_get_method = {
            istring("get"),
            {reinterpret_cast< RTTI::staticarray< const RTTI::Method::Overload >* >(&s_method_get_overloads)},
            {&s_get_method}
        };
        be_section(rtti_object)
        static RTTI::ObjectInfo valueGet = {
            {0},
            {0},
            s_get_method.name,
            RTTI::Value(s_get_method)
        };
        result->objects.set(&valueGet);
        be_section(rtti_object)
        static RTTI::ObjectInfo s_object = {
            ::BugEngine::be_game_Namespace_BugEngine_Settings()->objects,
            {0},
            name(),
            RTTI::Value(result)
        };
        be_game_Namespace_BugEngine_Settings()->objects.set(&s_object);;
        return result;
    }

};

}

/**************************************************************************************************/
#endif
