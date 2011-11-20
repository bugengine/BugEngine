/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_CLASSINFO_HH_
#define BE_RTTI_CLASSINFO_HH_
/*****************************************************************************/
#include    <rtti/value.hh>
#include    <core/runtime/md5.hh>

namespace BugEngine { namespace RTTI
{

struct PropertyInfo;
struct MethodInfo;
struct TagInfo;

struct be_api(RTTI) ClassInfo
{
    friend struct BugEngine::TypeInfo;
    friend class BugEngine::Value;
published:
    struct ObjectInfo
    {
        raw<const ObjectInfo> const     next;
        istring const                   name;
        Value                           value;
    };
published:
    inamespace const                    name;
    raw<const ClassInfo> const          parent;
    u32 const                           size;
    i32 const                           offset;
    raw<TagInfo>                        tags;
    raw<const PropertyInfo> const       properties;
    raw<const MethodInfo> const         methods;
    raw<const ObjectInfo>               objects;
    raw<const MethodInfo> const         constructor;
    raw<const MethodInfo> const         call;
public:
    typedef void (*CopyConstructor)(const void *, void *);
    typedef void (*Destructor)(void *);
    const CopyConstructor   copyconstructor;
    const Destructor        destructor;
    const MD5               hash;
published:
    Value getTag(const TypeInfo& type) const;
    Value getTag(raw<const ClassInfo> type) const;

    Value get(Value& from, istring name) const;

    bool isA(raw<const ClassInfo> klass) const;

    Value create(Value* params, u32 nparams) const;
public:
    typedef void(*EnumerateCallback)(const Value& v);
    enum EnumerateRecursion
    {
        EnumerateOwn,
        EnumerateRecursive
    };
    void enumerateObjects(EnumerateRecursion recursion, EnumerateCallback callback) const;
    u32 distance(raw<const ClassInfo> other) const;
    const ObjectInfo* addObject(const istring& s, const ObjectInfo* ob);
private: // friend Value
    void copy(const void* src, void* dst) const;
    void destroy(void* src) const;
};

}

be_api(RTTI) raw<RTTI::ClassInfo> be_game_Namespace();
raw<RTTI::ClassInfo> be_game_Namespace_BugEngine();

#define BE_REGISTER_NAMESPACE_1(plugin, n)                                                                                                          \
namespace BugEngine                                                                                                                                 \
{                                                                                                                                                   \
    raw<BugEngine::RTTI::ClassInfo> be_##plugin##_Namespace_##n()                                                                                   \
    {                                                                                                                                               \
        static RTTI::ClassInfo ci = { #n, {0}, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0, {{ 0, 0, 0, 0 }} };                                        \
        static RTTI::ClassInfo::ObjectInfo ob = { be_##plugin##_Namespace()->objects, #n, Value(&ci) };                                             \
        static raw<const RTTI::ClassInfo::ObjectInfo> result = {be_##plugin##_Namespace()->objects.set(&ob)};                                       \
        be_forceuse(result);                                                                                                                        \
        raw<BugEngine::RTTI::ClassInfo> ptr = {&ci};                                                                                                \
        return ptr;                                                                                                                                 \
    }                                                                                                                                               \
    static raw<BugEngine::RTTI::ClassInfo> s_##plugin##_##n = be_##plugin##_Namespace_##n();                                                        \
}

#define BE_REGISTER_NAMESPACE_2(plugin, n1, n2)                                                                                                     \
namespace BugEngine                                                                                                                                 \
{                                                                                                                                                   \
    raw<BugEngine::RTTI::ClassInfo> be_##plugin##_Namespace_##n1##_##n2()                                                                           \
    {                                                                                                                                               \
        static RTTI::ClassInfo ci = { #n1"."#n2, {0}, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0, {{ 0, 0, 0, 0 }} };                                 \
        static RTTI::ClassInfo::ObjectInfo ob = { be_##plugin##_Namespace_##n1()->objects, #n2, Value(&ci) };                                       \
        static raw<const RTTI::ClassInfo::ObjectInfo> result = {be_##plugin##_Namespace_##n1()->objects.set(&ob)};                           \
        be_forceuse(result);                                                                                                                        \
        raw<BugEngine::RTTI::ClassInfo> ptr = {&ci};                                                                                                \
        return ptr;                                                                                                                                 \
    }                                                                                                                                               \
    static raw<BugEngine::RTTI::ClassInfo> s_##plugin##_##n1##_##n2=be_##plugin##_Namespace_##n1##_##n2();                                          \
}

#define BE_REGISTER_NAMESPACE_3(plugin, n1, n2, n3)                                                                                                 \
namespace BugEngine                                                                                                                                 \
{                                                                                                                                                   \
    raw<BugEngine::RTTI::ClassInfo> be_##plugin##_Namespace_##n1##_##n2##_##n3()                                                                    \
    {                                                                                                                                               \
        static RTTI::ClassInfo ci = { #n1"."#n2"."#n3, {0}, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0, {{ 0, 0, 0, 0 }} };                           \
        static RTTI::ClassInfo::ObjectInfo ob = { be_##plugin##_Namespace_##n1##_##n2()->objects, #n3, Value(&ci) };                                \
        static raw<const RTTI::ClassInfo::ObjectInfo> result = {be_##plugin##_Namespace_##n1##_##n2()->objects.set(&ob)};                           \
        be_forceuse(result);                                                                                                                        \
        raw<BugEngine::RTTI::ClassInfo> ptr = {&ci};                                                                                                \
        return ptr;                                                                                                                                 \
    }                                                                                                                                               \
    static raw<BugEngine::RTTI::ClassInfo> s_##plugin##_##n1##_##n2##_##n3=be_##plugin##_Namespace_##n1##_##n2##_##n3();                            \
}
#define BE_REGISTER_NAMESPACE_4(plugin, n1, n2, n3, n4)                                                                                             \
namespace BugEngine                                                                                                                                 \
{                                                                                                                                                   \
    raw<BugEngine::RTTI::ClassInfo> be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4()                                                             \
    {                                                                                                                                               \
        static RTTI::ClassInfo ci = { #n1"."#n2"."#n3"."#n4, {0}, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0, {{ 0, 0, 0, 0 }} };                     \
        static RTTI::ClassInfo::ObjectInfo ob = { be_##plugin##_Namespace_##n1##_##n2##_##n3()->objects, #n4, Value(&ci) };                         \
        static raw<const RTTI::ClassInfo::ObjectInfo> result = {be_##plugin##_Namespace_##n1##_##n2##_##n3()->objects.set(&ob)};                    \
        static raw<const RTTI::ClassInfo::ObjectInfo> result = be_##plugin##_Namespace_##n1##_##n2##_##n3()->objects = {&ob};                       \
        be_forceuse(result);                                                                                                                        \
        raw<BugEngine::RTTI::ClassInfo> ptr = {&ci};                                                                                                \
        return ptr;                                                                                                                                 \
    }                                                                                                                                               \
    static raw<BugEngine::RTTI::ClassInfo> s_##plugin##_##n1##_##n2##_##n3##_##n4=be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4();              \
}

#define BE_REGISTER_NAMESPACE_5(plugin,n1, n2, n3, n4, n5)                                                                                          \
namespace BugEngine                                                                                                                                 \
{                                                                                                                                                   \
    raw<BugEngine::RTTI::ClassInfo> be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4##_##n5()                                                      \
    {                                                                                                                                               \
        static RTTI::ClassInfo ci = { #n1"."#n2"."#n3"."#n4"."#n5, {0}, 0, 0, {0}, {0}, {0}, {0}, {0}, {0}, 0, 0, {{ 0, 0, 0, 0 }} };               \
        static RTTI::ClassInfo::ObjectInfo ob = { be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4()->objects, #n5, Value(&ci) };                  \
        static raw<const RTTI::ClassInfo::ObjectInfo> result = {be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4()->objects.set(&ob)};             \
        be_forceuse(result);                                                                                                                        \
        raw<BugEngine::RTTI::ClassInfo> ptr = {&ci};                                                                                                \
        return ptr;                                                                                                                                 \
    }                                                                                                                                               \
    static raw<BugEngine::RTTI::ClassInfo> s_##plugin##_##n1##_##n2##_##n3##_##n4##_##n5=be_##plugin##_Namespace_##n1##_##n2##_##n3##_##n4##_##n5();\
}

}

/*****************************************************************************/
#endif

