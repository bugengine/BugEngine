/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_HELPER_STATICARRAY_FACTORY_HH_
#define BE_RTTI_ENGINE_HELPER_STATICARRAY_FACTORY_HH_
/**************************************************************************************************/
#include    <rtti/stdafx.h>
#include    <rtti/engine/helper/staticarray.hh>
#include    <rtti/value.hh>
#include    <rtti/typeinfo.hh>


namespace BugEngine
{

template< typename T >
struct be_typeid< RTTI::staticarray<T> >
{
    static const RTTI::Type value_type;
    static BE_EXPORT raw<RTTI::Class> preklass();
    static inline istring name();
    static raw<const RTTI::Class> klass();
    static RTTI::Type  type();
    static BE_EXPORT raw<const RTTI::Class> s_initialisation;
private:
    static u32 array_size(const RTTI::Value& v);
    static RTTI::Value index(RTTI::Value& v, u32 i);
    static RTTI::Value indexConst(const RTTI::Value& v, u32 i);
    static RTTI::Value callStaticArrayOperatorIndex(RTTI::Value* params, u32 paramCount);
    static RTTI::Value callStaticArraySize(RTTI::Value* params, u32 paramCount);
    static RTTI::Value callStaticArrayOperatorIndexConst(RTTI::Value* params, u32 paramCount);
    static BE_EXPORT raw<const RTTI::Class> registerProperties();
};

}

/**************************************************************************************************/
#endif
