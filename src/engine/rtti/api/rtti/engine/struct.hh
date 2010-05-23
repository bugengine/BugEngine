/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_STRUCT_HH_
#define BE_RTTI_ENGINE_STRUCT_HH_
/*****************************************************************************/
#include   <rtti/engine/class.hh>
#include   <rtti/classinfo.hh>
#include   <rtti/propertyinfo.hh>

namespace BugEngine { namespace RTTI
{

template< typename Class, typename Parent = void >
class StructInfo : public BugEngine::ClassInfo
{
public:
    StructInfo(const inamespace& name);
    virtual ~StructInfo();

    template< typename T >
    PropertyInfo& property(const istring& name, T(Class::*member));

    const TypeInfo* parent() const override { return be_typeid<Parent>(); }
    size_t          size() const override   { return sizeof(Class); }
    void            copy(const void* source, void* dest) const override { new(dest) Class(*reinterpret_cast<const Class*>(source)); }
    void            destroy(void* dest) const override { reinterpret_cast<Class*>(dest)->~Class(); }
};

}}

#include   <rtti/engine/struct.inl>
#include   <rtti/documentation/brief.hh>
#include   <rtti/documentation/documentation.hh>


#define be_struct(klass,parent,name)                                           \
    template<>                                                                  \
    BE_EXPORT const TypeInfo* be_typeid<klass>();                               \
    static const TypeInfo* BE_CONCAT(t,__LINE__) = be_typeid<klass>();          \
    template<>                                                                  \
    BE_EXPORT const TypeInfo* be_typeid<klass>()                                \
    {                                                                           \
        typedef klass KLASS;                                                    \
        static RTTI::StructInfo<klass, parent> result(name);                    \
        static TypeInfo& r = result

/*****************************************************************************/
#endif
