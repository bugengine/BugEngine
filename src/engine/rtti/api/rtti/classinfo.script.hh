/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_CLASSINFO_HH_
#define BE_RTTI_CLASSINFO_HH_
/*****************************************************************************/
#include    <rtti/value.hh>

namespace BugEngine { namespace RTTI
{

struct PropertyInfo;
struct MethodInfo;


struct be_api(RTTI) ClassInfo
{
    friend struct BugEngine::TypeInfo;
    friend class BugEngine::Value;
published:
    class MetaClassInfo;
    friend class MetaClassInfo;
published:
    inamespace          name;
    const ClassInfo*    parent;
    u32                 size;
    i32                 offset;
    const PropertyInfo* properties;
    const MethodInfo*   methods;
    const MethodInfo*   constructor;
    const MethodInfo*   call;
private:
    /* list of tags is const but each tag can be changed */
    //mutable minitl::vector<Value>                   m_tags;
public:
    void (*copyconstructor)(const void* src, void* dst);
    void (*destructor)(void* src);
published:
    Value get(Value& from, istring name) const;

    Value getTag(const TypeInfo& type) const;
    Value getTag(const ClassInfo* type) const;

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

