/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_CLASSINFO_SCRIPT_HH_
#define BE_RTTI_CLASSINFO_SCRIPT_HH_
/*****************************************************************************/
#include    <rtti/value.hh>
#include    <core/runtime/md5.hh>

namespace BugEngine { namespace RTTI
{

struct Property;
struct Method;
struct Tag;

struct be_api(RTTI) Class
{
    friend struct Type;
    friend class Value;
published:
    struct ObjectInfo
    {
        raw<const ObjectInfo> const     next;
        raw<Tag>                        tags;
        istring const                   name;
        Value                           value;
    };
published:

    inamespace const            name;

    raw<const Class> const      parent;

    u16 const                   size;

    u16 const                   align;

    i32 const                   offset;

    raw<Tag>                    tags;

    raw<const Property> const   properties;

    raw<const Method>           methods;

    raw<const ObjectInfo>       objects;

    be_tag(Alias("?call"))
    raw<const Method> const     constructor;

    raw<const Method> const     cast;
public:

    typedef void (*CopyConstructor)(const void* source, void* destination);
    typedef void (*Destructor)(void* object);
    const CopyConstructor   copyconstructor;
    const Destructor        destructor;
published:
    Value getTag(const Type& type) const;
    Value getTag(raw<const Class> type) const;

    Value get(Value& from, istring name) const;

    bool isA(raw<const Class> klass) const;
public:
    typedef void(*EnumerateCallback)(const Value& v);
    enum EnumerateRecursion
    {
        EnumerateOwn,
        EnumerateRecursive
    };
    void enumerateObjects(EnumerateRecursion recursion, EnumerateCallback callback) const;
    u32 distance(raw<const Class> other) const;
    const ObjectInfo* addObject(const istring& s, const ObjectInfo* ob);
private: // friend Value
    void copy(const void* src, void* dst) const;
    void destroy(void* src) const;
};

}

be_api(RTTI) raw<RTTI::Class> be_game_Namespace();
raw<RTTI::Class> be_game_Namespace_BugEngine();

}

#define BE_REGISTER_NAMESPACE_1_NAMED(plugin, n)                                                                                                \
namespace BugEngine                                                                                                                             \
{                                                                                                                                               \
    raw<BugEngine::RTTI::Class> be_##plugin##_Namespace_##n()                                                                                   \
    {                                                                                                                                           \
        static RTTI::Class ci = { #n, {0}, 0, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0 };                                                       \
        static RTTI::Class::ObjectInfo ob = { be_##plugin##_Namespace()->objects, {0}, #n, RTTI::Value(&ci) };                                  \
        static raw<const RTTI::Class::ObjectInfo> result = {be_##plugin##_Namespace()->objects.set(&ob)};                                       \
        be_forceuse(result);                                                                                                                    \
        raw<BugEngine::RTTI::Class> ptr = {&ci};                                                                                                \
        return ptr;                                                                                                                             \
    }                                                                                                                                           \
    static raw<BugEngine::RTTI::Class> s_##plugin##_##n = be_##plugin##_Namespace_##n();                                                        \
}

#define BE_REGISTER_NAMESPACE_2_NAMED(plugin, n1, n2)                                                                                           \
namespace BugEngine                                                                                                                             \
{                                                                                                                                               \
    raw<BugEngine::RTTI::Class> be_##plugin##_Namespace_##n1##_##n2()                                                                           \
    {                                                                                                                                           \
        static RTTI::Class ci = { #n1"."#n2, {0}, 0, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0 };                                                \
        static RTTI::Class::ObjectInfo ob = { be_##plugin##_Namespace_##n1()->objects, {0}, #n2, RTTI::Value(&ci) };                            \
        static raw<const RTTI::Class::ObjectInfo> result = {be_##plugin##_Namespace_##n1()->objects.set(&ob)};                                  \
        be_forceuse(result);                                                                                                                    \
        raw<BugEngine::RTTI::Class> ptr = {&ci};                                                                                                \
        return ptr;                                                                                                                             \
    }                                                                                                                                           \
    static raw<BugEngine::RTTI::Class> s_##plugin##_##n1##_##n2=be_##plugin##_Namespace_##n1##_##n2();                                          \
}

#define BE_REGISTER_NAMESPACE_3_NAMED(plugin, n1, n2, n3)                                                                                       \
namespace BugEngine                                                                                                                             \
{                                                                                                                                               \
    raw<BugEngine::RTTI::Class> be_##plugin##_Namespace_##n1##_##n2##_##n3()                                                                    \
    {                                                                                                                                           \
        static RTTI::Class ci = { #n1"."#n2"."#n3, {0}, 0, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0 };                                          \
        static RTTI::Class::ObjectInfo ob = { be_##plugin##_Namespace_##n1##_##n2()->objects, {0}, #n3, RTTI::Value(&ci) };                     \
        static raw<const RTTI::Class::ObjectInfo> result = {be_##plugin##_Namespace_##n1##_##n2()->objects.set(&ob)};                           \
        be_forceuse(result);                                                                                                                    \
        raw<BugEngine::RTTI::Class> ptr = {&ci};                                                                                                \
        return ptr;                                                                                                                             \
    }                                                                                                                                           \
    static raw<BugEngine::RTTI::Class> s_##plugin##_##n1##_##n2##_##n3=be_##plugin##_Namespace_##n1##_##n2##_##n3();                            \
}
#define BE_REGISTER_NAMESPACE_4_NAMED(plugin, n1, n2, n3, n4)                                                                                   \
namespace BugEngine                                                                                                                             \
{                                                                                                                                               \
    raw<BugEngine::RTTI::Class> be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4()                                                             \
    {                                                                                                                                           \
        static RTTI::Class ci = { #n1"."#n2"."#n3"."#n4, {0}, 0, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0 };                                    \
        static RTTI::Class::ObjectInfo ob = { be_##plugin##_Namespace_##n1##_##n2##_##n3()->objects, {0}, #n4, RTTI::Value(&ci) };              \
        static raw<const RTTI::Class::ObjectInfo> result = {be_##plugin##_Namespace_##n1##_##n2##_##n3()->objects.set(&ob)};                    \
        static raw<const RTTI::Class::ObjectInfo> result = be_##plugin##_Namespace_##n1##_##n2##_##n3()->objects = {&ob};                       \
        be_forceuse(result);                                                                                                                    \
        raw<BugEngine::RTTI::Class> ptr = {&ci};                                                                                                \
        return ptr;                                                                                                                             \
    }                                                                                                                                           \
    static raw<BugEngine::RTTI::Class> s_##plugin##_##n1##_##n2##_##n3##_##n4=be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4();              \
}

#define BE_REGISTER_NAMESPACE_5_NAMED(plugin,n1, n2, n3, n4, n5)                                                                                \
namespace BugEngine                                                                                                                             \
{                                                                                                                                               \
    raw<BugEngine::RTTI::Class> be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4##_##n5()                                                      \
    {                                                                                                                                           \
        static RTTI::Class ci = { #n1"."#n2"."#n3"."#n4"."#n5, {0}, 0, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0 };                              \
        static RTTI::Class::ObjectInfo ob = { be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4()->objects, {0}, #n5, RTTI::Value(&ci) };       \
        static raw<const RTTI::Class::ObjectInfo> result = {be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4()->objects.set(&ob)};             \
        be_forceuse(result);                                                                                                                    \
        raw<BugEngine::RTTI::Class> ptr = {&ci};                                                                                                \
        return ptr;                                                                                                                             \
    }                                                                                                                                           \
    static raw<BugEngine::RTTI::Class> s_##plugin##_##n1##_##n2##_##n3##_##n4##_##n5=be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4##_##n5();\
}


#define BE_REGISTER_NAMESPACE_1_(plugin, n1)                            \
    BE_REGISTER_NAMESPACE_1_NAMED(plugin, n1)
#define BE_REGISTER_NAMESPACE_2_(plugin, n1, n2)                        \
    BE_REGISTER_NAMESPACE_2_NAMED(plugin, n1, n2)
#define BE_REGISTER_NAMESPACE_3_(plugin, n1, n2, n3)                    \
    BE_REGISTER_NAMESPACE_3_NAMED(plugin, n1, n2, n3)
#define BE_REGISTER_NAMESPACE_4_(plugin, n1, n2, n3, n4)                \
    BE_REGISTER_NAMESPACE_4_NAMED(plugin, n1, n2, n3, n4)
#define BE_REGISTER_NAMESPACE_5_(plugin, n1, n2, n3, n4, n5)            \
    BE_REGISTER_NAMESPACE_5_NAMED(plugin, n1, n2, n3, n4, n5)


#define BE_REGISTER_NAMESPACE_1(n1)                                     \
    BE_REGISTER_NAMESPACE_1_(BE_PROJECTSHORTNAME, n1)
#define BE_REGISTER_NAMESPACE_2(n1, n2)                                 \
    BE_REGISTER_NAMESPACE_2_(BE_PROJECTSHORTNAME, n1, n2)
#define BE_REGISTER_NAMESPACE_3(n1, n2, n3)                             \
    BE_REGISTER_NAMESPACE_3_(BE_PROJECTSHORTNAME, n1, n2, n3)
#define BE_REGISTER_NAMESPACE_4(n1, n2, n3, n4)                         \
    BE_REGISTER_NAMESPACE_4_(BE_PROJECTSHORTNAME, n1, n2, n3, n4)
#define BE_REGISTER_NAMESPACE_5(n1, n2, n3, n4, n5)                     \
    BE_REGISTER_NAMESPACE_5_(BE_PROJECTSHORTNAME, n1, n2, n3, n4, n5)

/*****************************************************************************/
#endif

