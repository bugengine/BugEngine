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
        const ObjectInfo* const next;
        istring const           name;
        Value                   value;
    };
published:
    inamespace const                        name;
    const ClassInfo* const                  parent;
    u32 const                               size;
    i32 const                               offset;
    TagInfo*                                tags;
    const PropertyInfo* const               properties;
    const MethodInfo* const                 methods;
    const ObjectInfo*                       objects;
    const MethodInfo* const                 constructor;
    const MethodInfo* const                 call;
public:
    typedef void (*CopyConstructor)(const void *, void *);
    typedef void (*Destructor)(void *);
    const CopyConstructor   copyconstructor;
    const Destructor        destructor;
    const MD5               hash;
published:
    Value getTag(const TypeInfo& type) const;
    Value getTag(const ClassInfo* type) const;

    Value get(Value& from, istring name) const;

    bool isA(const ClassInfo* klass) const;

    Value operator()(Value* params, u32 nparams) const;
public:
    u32 distance(const ClassInfo* other) const;
    const ObjectInfo* addObject(const istring& s, const ObjectInfo* ob);
private: // friend Value
    void copy(const void* src, void* dst) const;
    void destroy(void* src) const;
};

}

BE_EXPORT RTTI::ClassInfo* be_Namespace();
RTTI::ClassInfo* be_Namespace_BugEngine();
#define BE_REGISTER_NAMESPACE_1(name)                                                                                                           \
namespace BugEngine                                                                                                                             \
{                                                                                                                                               \
    BugEngine::RTTI::ClassInfo* be_Namespace_##name()                                                                                           \
    {                                                                                                                                           \
        static RTTI::ClassInfo ci = { #name, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {{ 0, 0, 0, 0 }} };                                               \
        static RTTI::ClassInfo::ObjectInfo ob = { be_Namespace()->objects, #name, Value(&ci) };                                                 \
        static const RTTI::ClassInfo::ObjectInfo* result = (be_Namespace()->objects = &ob);                                                     \
        be_forceuse(result);                                                                                                                    \
        return &ci;                                                                                                                             \
    }                                                                                                                                           \
}

#define BE_REGISTER_NAMESPACE_2(name1, name2)                                                                                                   \
namespace BugEngine                                                                                                                             \
{                                                                                                                                               \
    BugEngine::RTTI::ClassInfo* be_Namespace_##name1##_##name2()                                                                                \
    {                                                                                                                                           \
        static RTTI::ClassInfo ci = { #name1 "." #name2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {{ 0, 0, 0, 0 }} };                                   \
        static RTTI::ClassInfo::ObjectInfo ob = { be_Namespace_##name1()->objects, #name2, Value(&ci) };                                        \
        static const RTTI::ClassInfo::ObjectInfo* result = (be_Namespace_##name1()->objects = &ob);                                             \
        be_forceuse(result);                                                                                                                    \
        return &ci;                                                                                                                             \
    }                                                                                                                                           \
}

#define BE_REGISTER_NAMESPACE_3(name1, name2, name3)                                                                                            \
namespace BugEngine                                                                                                                             \
{                                                                                                                                               \
    BugEngine::RTTI::ClassInfo* be_Namespace_##name1##_##name2##_##name3()                                                                      \
    {                                                                                                                                           \
        static RTTI::ClassInfo ci = { #name1 "." #name2 "." #name3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {{ 0, 0, 0, 0 }} };                        \
        static RTTI::ClassInfo::ObjectInfo ob = { be_Namespace_##name1##_##name2()->objects, #name3, Value(&ci) };                              \
        static const RTTI::ClassInfo::ObjectInfo* result = (be_Namespace_##name1##_##name2()->objects = &ob);                                   \
        be_forceuse(result);                                                                                                                    \
        return &ci;                                                                                                                             \
    }                                                                                                                                           \
}
#define BE_REGISTER_NAMESPACE_4(name1, name2, name3, name4)                                                                                     \
namespace BugEngine                                                                                                                             \
{                                                                                                                                               \
    BugEngine::RTTI::ClassInfo* be_Namespace_##name1##_##name2##_##name3##_##name4()                                                            \
    {                                                                                                                                           \
        static RTTI::ClassInfo ci = { #name1 "." #name2 "." #name3 "." #name4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {{ 0, 0, 0, 0 }} };             \
        static RTTI::ClassInfo::ObjectInfo ob = { be_Namespace_##name1##_##name2##_##name3()->objects, #name4, Value(&ci) };                    \
        static const RTTI::ClassInfo::ObjectInfo* result = (be_Namespace_##name1##_##name2##_##name3()->objects = &ob);                         \
        be_forceuse(result);                                                                                                                    \
        return &ci;                                                                                                                             \
    }                                                                                                                                           \
}

#define BE_REGISTER_NAMESPACE_5(name1, name2, name3, name4, name5)                                                                              \
namespace BugEngine                                                                                                                             \
{                                                                                                                                               \
    BugEngine::RTTI::ClassInfo* be_Namespace_##name1##_##name2##_##name3##_##name4##_##name5()                                                  \
    {                                                                                                                                           \
        static RTTI::ClassInfo ci = { #name1 "." #name2 "." #name3 "." #name4 "." #name5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, {{ 0, 0, 0, 0 }} };  \
        static RTTI::ClassInfo::ObjectInfo ob = { be_Namespace_##name1##_##name2##_##name3##_##name4()->objects, #name5, Value(&ci) };          \
        static const RTTI::ClassInfo::ObjectInfo* result = (be_Namespace_##name1##_##name2##_##name3##_##name4()->objects = &ob);               \
        be_forceuse(result);                                                                                                                    \
        return &ci;                                                                                                                             \
    }                                                                                                                                           \
}

}

/*****************************************************************************/
#endif

