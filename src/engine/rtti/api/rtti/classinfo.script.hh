/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_CLASSINFO_HH_
#define BE_RTTI_CLASSINFO_HH_
/*****************************************************************************/
#include    <rtti/namespace.script.hh>
#include    <rtti/engine/propertyinfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/value.hh>


namespace BugEngine
{
struct TypeInfo;
}

namespace BugEngine { namespace RTTI
{

struct PropertyInfo;
struct MethodInfo;


be_meta(ClassInfo::MetaClassInfo)
class be_api(RTTI) ClassInfo : public Namespace
{
    friend struct BugEngine::TypeInfo;
    friend class Namespace;
    friend class BugEngine::Value;
published:
    class be_api(RTTI) MetaClassInfo;
    friend class MetaClassInfo;
published:
    const inamespace                                name;
    const ref<const ClassInfo>                      parent;
    const u32                                       size;
    const i32                                       offset;
    minitl::hashmap< istring, const PropertyInfo>   properties;
    minitl::hashmap< istring, const MethodInfo>     methods;
    minitl::vector<Value>                           tags;
    MethodInfo                                      constructor;
    MethodInfo                                      call;
public:
    void (*copyconstructor)(const void* src, void* dst);
    void (*destructor)(void* src);
published:
    ClassInfo(const inamespace& name, ref<const ClassInfo> parent);
    ClassInfo(const inamespace& name, ref<const ClassInfo> parent, u32 size, i32 offset);
    ~ClassInfo();

published:
    void addProperty(istring name, const PropertyInfo& prop);
    void removeProperty(istring name);
    void addMethod(istring name, const MethodInfo& method);
    void removeMethod(istring name);

    virtual Value get(Value& from, istring name) const;

    Value getTag(const TypeInfo& type) const;
    bool isA(weak<const ClassInfo> klass) const;

    Value operator()(Value* params, size_t nparams) const;
private: // friend Value
    void copy(const void* src, void* dst) const;
    void destroy(void* src) const;
};

}}

#include    <rtti/engine/metaclassinfo.script.hh>

/*****************************************************************************/
#endif

