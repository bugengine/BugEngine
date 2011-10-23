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
    TagInfo* const                          tags;
    const PropertyInfo* const               properties;
    const MethodInfo* const                 constructor;
    const MethodInfo* const                 call;
    const ObjectInfo* const                 objects;
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
private: // friend Value
    void copy(const void* src, void* dst) const;
    void destroy(void* src) const;
};

}

BE_EXPORT RTTI::ClassInfo* be_Namespace();

}

/*****************************************************************************/
#endif

