/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_ENUM_HH_
#define BE_RTTI_ENGINE_ENUM_HH_
/*****************************************************************************/
#include   <rtti/enuminfo.hh>
#include   <rtti/propertyinfo.hh>

namespace BugEngine { namespace RTTI
{

template< typename Enum >
class EnumInfo : public BugEngine::EnumInfo
{
public:
    EnumInfo(const inamespace& name);
    virtual ~EnumInfo();

    PropertyInfo& value(istring name, Enum value);

    const TypeInfo* parent() const override { return be_typeid<void>(); }
    size_t          size() const override   { return sizeof(Enum); }
    void            copy(const void* source, void* dest) const override { new(dest) Enum(*reinterpret_cast<const Enum*>(source)); }
    void            destroy(void* /*dest*/) const override { }
};

}}

#include   <rtti/engine/enum.inl>
#include   <rtti/documentation/brief.hh>
#include   <rtti/documentation/documentation.hh>


#define be_enum(klass,name)                                                    \
    template<>                                                                  \
    BE_EXPORT const TypeInfo* be_typeid<klass>();                               \
    static const TypeInfo* BE_CONCAT(t,__LINE__) = be_typeid<klass>();          \
    template<>                                                                  \
    BE_EXPORT const TypeInfo* be_typeid<klass>()                                \
    {                                                                           \
        typedef klass KLASS;                                                    \
        static RTTI::EnumInfo<klass> result(name);                              \
        static TypeInfo& r = result

#define be_value(name)                                                         \
        ;static BugEngine::PropertyInfo& p##name = result.value(#name, name)
#define be_value_name(v, name)                                                 \
        ;static BugEngine::PropertyInfo& p##name = result.value(#name, v)

/*****************************************************************************/
#endif
