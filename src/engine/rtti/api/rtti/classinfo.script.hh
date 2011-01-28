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
    friend class Value;
published:
    const inamespace                                    name;
    const ref<const ClassInfo>                          parent;
    const u32                                           size;
    const i32                                           offset;
    minitl::hashmap< istring, ref<const PropertyInfo> > properties;
    ref<MethodInfo>                                     call;
    ref<MethodInfo>                                     index;
public:
    ClassInfo(const inamespace& name, ref<const ClassInfo> parent);
    ClassInfo(const inamespace& name, ref<const ClassInfo> parent, ref<ClassInfo> metaclass, u32 size, i32 offset);
    ~ClassInfo();

    void addProperty(const istring& name, ref<const PropertyInfo> prop);
    void removeProperty(const istring& name);
    weak<const PropertyInfo> getProperty(const istring& name) const;

    bool isA(weak<const ClassInfo> klass) const;
public:
    Value operator()(Value* params, size_t nparams) const;
private: // friend Value
    void copy(const void* src, void* dst) const;
    void destroy(void* src) const;
};

}}

/*****************************************************************************/
#endif

