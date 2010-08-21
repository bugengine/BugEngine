/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/typeinfo.hh>

namespace BugEngine
{

template< typename T >
class BuiltinClassInfo : public ClassInfo
{
public:
    BuiltinClassInfo(const inamespace& name);
    ~BuiltinClassInfo();

    size_t size() const override;
};

template< typename T >
BuiltinClassInfo<T>::BuiltinClassInfo(const inamespace& name)
:   ClassInfo(name)
{
}

template< typename T >
BuiltinClassInfo<T>::~BuiltinClassInfo()
{
}

template< typename T >
size_t BuiltinClassInfo<T>::size() const
{
    return sizeof(T);
}

#define BE_BUILTIN_TYPE(t)                                                         \
template<>                                                                          \
BE_EXPORT weak<const ClassInfo> be_typeid<t>::klass()                               \
{                                                                                   \
    static ref< BuiltinClassInfo<t> > ci = ref< BuiltinClassInfo<t> >::create<Arena::General>(#t);  \
    return ci;                                                                      \
}

BE_BUILTIN_TYPE(bool);
BE_BUILTIN_TYPE(u8);
BE_BUILTIN_TYPE(u16);
BE_BUILTIN_TYPE(u32);
BE_BUILTIN_TYPE(u64);
BE_BUILTIN_TYPE(i8);
BE_BUILTIN_TYPE(i16);
BE_BUILTIN_TYPE(i32);
BE_BUILTIN_TYPE(i64);
BE_BUILTIN_TYPE(float);
BE_BUILTIN_TYPE(double);



}
