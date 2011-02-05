/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_CLASSINFO_HH_
#define BE_RTTI_CLASSINFO_HH_
/*****************************************************************************/
#include    <rtti/namespace.script.hh>

namespace BugEngine
{
struct TypeInfo;
class Value;
}

namespace BugEngine { namespace RTTI
{

class PropertyInfo;
class MethodInfo;

class be_api(RTTI) ClassInfo : public Namespace
{
    friend struct BugEngine::TypeInfo;
    friend class Namespace;
    friend class BugEngine::Value;
published:
    const inamespace                                    name;
    const ref<const ClassInfo>                          parent;
    const u32                                           size;
    const i32                                           offset;
    minitl::hashmap< istring, ref<const PropertyInfo> > properties;
public:
    ref<const MethodInfo>                               callOperator;
public:
    ClassInfo(const inamespace& name, ref<const ClassInfo> parent);
    ClassInfo(const inamespace& name, ref<const ClassInfo> parent, ref<ClassInfo> metaclass, u32 size, i32 offset);
    ~ClassInfo();

    void addProperty(const istring& name, ref<const PropertyInfo> prop);
    void replaceProperty(const istring& name, ref<const PropertyInfo> prop);
    void removeProperty(const istring& name);
    weak<const PropertyInfo> getProperty(const istring& name) const;
published:
    bool isA(weak<const ClassInfo> klass) const;
published:
    void test(weak<const ClassInfo> p1, weak<const ClassInfo> p2) const;
    void test(weak<const ClassInfo> p1) const;
public:
    Value call(Value* params, size_t nparams) const;
private: // friend Value
    void copy(const void* src, void* dst) const;
    void destroy(void* src) const;
};

}}

/*****************************************************************************/
#endif

