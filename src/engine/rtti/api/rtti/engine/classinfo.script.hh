/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_ENGINE_CLASSINFO_SCRIPT_HH_
#define BE_RTTI_ENGINE_CLASSINFO_SCRIPT_HH_
/*****************************************************************************/

namespace BugEngine
{
struct TypeInfo;
}

namespace BugEngine { namespace RTTI
{

class PropertyInfo;
class MethodInfo;

class ClassInfo : public minitl::refcountable
{
    friend struct BugEngine::TypeInfo;
public:
    const inamespace                                    name;
    const ref<const ClassInfo>                          parent;
    const ref<const ClassInfo>                          metaclass;
    const u32                                           size;
private:
    minitl::hashmap< istring, ref<const PropertyInfo> > m_properties;
    minitl::hashmap< istring, ref<const MethodInfo> >   m_methods;
    ref<const MethodInfo>                               m_constructor;
    ref<const MethodInfo>                               m_destructor;
public:
    ClassInfo(const inamespace& name, ref<const ClassInfo> parent, ref<const ClassInfo> metaclass, u32 size);
    ~ClassInfo();

    ref<const PropertyInfo> operator[](const istring& prop) const;
private:
    void copy(const void* src, void* dst) const;
    void destroy(void* src) const;
};

}}

/*****************************************************************************/
#endif
