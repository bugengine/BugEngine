/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_CLASSINFO_SCRIPT_HH_
#define BE_RTTI_ENGINE_CLASSINFO_SCRIPT_HH_
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

class Dummy
{
    u32 str;
protected:
    virtual ~Dummy(){}
};

class be_api(RTTI) ClassInfo : private Dummy
                             , public Namespace
                             , public minitl::intrusive_list<const ClassInfo>::item
{
    friend struct BugEngine::TypeInfo;
    friend class Namespace;
    friend class Value;
public:
    const inamespace                                    name;
    const ref<const ClassInfo>                          parent;
    const u32                                           size;
private:
    const i32                                           offset;
private:
    mutable minitl::intrusive_list<const ClassInfo>     m_children;
    minitl::hashmap< istring, ref<const PropertyInfo> > m_properties;
    bool                                                m_inTree;
public:
    ClassInfo(ref<const ClassInfo> parent, ref<ClassInfo> metaclass);
    ClassInfo(const inamespace& name, ref<const ClassInfo> parent, ref<ClassInfo> metaclass, u32 size, i32 offset);
    ~ClassInfo();

    void addProperty(const istring& name, ref<const PropertyInfo> prop);
    weak<const PropertyInfo> getProperty(const istring& name) const;

    bool isA(weak<const ClassInfo> klass) const;
private:
    void copy(const void* src, void* dst) const;
    void destroy(void* src) const;
};

}}

/*****************************************************************************/
#endif
