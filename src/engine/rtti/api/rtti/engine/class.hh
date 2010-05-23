/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_CLASS_HH_
#define BE_RTTI_ENGINE_CLASS_HH_
/*****************************************************************************/
#include   <rtti/classinfo.hh>
#include   <rtti/propertyinfo.hh>

namespace BugEngine { namespace RTTI
{

template< typename Class, typename Parent = void >
class ClassInfo : public BugEngine::ClassInfo
{
public:
    ClassInfo(const inamespace& name);
    virtual ~ClassInfo();

    template< typename T >
    PropertyInfo& property(const istring& name, T(Class::*member));

    const TypeInfo* parent() const override { return be_typeid<Parent>(); }
    size_t          size() const override   { return sizeof(Class); }
    void            copy(const void* /*source*/, void* /*dest*/) const override { be_assert(false, "no copy!"); }
    void            destroy(void* dest) const override { reinterpret_cast<Class*>(dest)->~Class(); }
};

}}

#include   <rtti/engine/class.inl>
#include   <rtti/documentation/brief.hh>
#include   <rtti/documentation/documentation.hh>


#define be_class(klass,parent,name)                                            \
    template<>                                                                  \
    const TypeInfo* be_typeid<klass>();                                         \
    static const TypeInfo* BE_CONCAT(t,__LINE__) = be_typeid<klass>();          \
    template<>                                                                  \
    const TypeInfo* be_typeid<klass>()                                          \
    {                                                                           \
        typedef klass KLASS;                                                    \
        static RTTI::ClassInfo<klass, parent> result(name);                     \
        static TypeInfo& r = result

#define be_field(name)                                                         \
        ;static BugEngine::PropertyInfo& p##name = result.property(#name, &KLASS::name)
#define be_field_name(var, name)                                                         \
        ;static BugEngine::PropertyInfo& p##name = result.property(#name, &KLASS::var)

#define be_end ;return &result; }

template< typename T >
TypeInfo be_typeid< ref<T> >()
{
    return TypeInfo(
}

template< typename T >
const TypeInfo* be_typeid< weak<T> >()
{
}

template< typename T >
const TypeInfo* be_typeid< const T >()
{
}

template< typename T >
const TypeInfo* be_typeid< T& >()
{
}

/*****************************************************************************/
#endif
