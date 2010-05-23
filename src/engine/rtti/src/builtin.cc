/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>
#include    <rtti/typeinfo.hh>

namespace BugEngine
{

template< typename T >
class BuiltinTypeInfo : public TypeInfo
{
public:
    BuiltinTypeInfo(const inamespace& name) : TypeInfo(name)            { }
    virtual ~BuiltinTypeInfo()                                          { }

    const TypeInfo* parent() const override                             { return be_typeid<void>(); }
    size_t          size() const override                               { return sizeof(T); }
    void            copy(const void* source, void* dest) const override { new(dest) T(*reinterpret_cast<const T*>(source)); }
    void            destroy(void* dest) const override                  { reinterpret_cast<T*>(dest)->~T(); }
private:
    BuiltinTypeInfo(const TypeInfo& other);
    BuiltinTypeInfo& operator=(const TypeInfo& other);
};

template<>
BE_EXPORT const TypeInfo* be_typeid<void>()
{
    return 0;
}

template<>
BE_EXPORT const TypeInfo* be_typeid<int>()
{
    static BuiltinTypeInfo<int> ti("int");
    return &ti;
}

template<>
BE_EXPORT const TypeInfo* be_typeid<message>()
{
    static BuiltinTypeInfo<message> ti("message");
    return &ti;
}

template<>
BE_EXPORT const TypeInfo* be_typeid<istring>()
{
    static BuiltinTypeInfo<istring> ti("istring");
    return &ti;
}

template<>
BE_EXPORT const TypeInfo* be_typeid<inamespace>()
{
    static BuiltinTypeInfo<inamespace> ti("inamespace");
    return &ti;
}

template<>
BE_EXPORT const TypeInfo* be_typeid<std::string>()
{
    static BuiltinTypeInfo<std::string> ti("string");
    return &ti;
}

}
