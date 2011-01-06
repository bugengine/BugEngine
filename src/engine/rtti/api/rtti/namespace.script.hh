/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_NAMESPACE_HH_
#define BE_RTTI_NAMESPACE_HH_
/*****************************************************************************/
#include    <rtti/engine/classinfo.script.hh>

namespace BugEngine
{
class Value;
}

namespace BugEngine { namespace RTTI
{

class Namespace : public minitl::refcountable
{
    friend class ::BugEngine::Value;
private:
    class PropertyInfo;
    friend class PropertyInfo;
private:
    ref<ClassInfo>  m_classInfo;
private:
    weak<Namespace> getOrCreateNamespace(const inamespace& name);
public:
    Namespace();
    ~Namespace();

    void add(const istring name, Value value);
    void registerClassRoot(weak<const ClassInfo> klass);
};

}}

/*****************************************************************************/
#endif
