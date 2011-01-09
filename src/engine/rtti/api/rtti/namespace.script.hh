/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_NAMESPACE_HH_
#define BE_RTTI_NAMESPACE_HH_
/*****************************************************************************/

namespace BugEngine
{
class Value;
}

namespace BugEngine { namespace RTTI
{

class ClassInfo;

class Namespace : public minitl::refcountable
{
    friend class ::BugEngine::Value;
private:
    class MetaPropertyInfo;
    friend class MetaPropertyInfo;
public:
    ref<ClassInfo> const    metaclass;
private:
    weak<const Namespace> getOrCreateNamespace(const inamespace& name);
protected:
    Namespace(ref<ClassInfo> metaclass);
public:
    Namespace();
    ~Namespace();

    void add(const istring name, Value value);
    void registerClassRoot(weak<const ClassInfo> klass);
};

}}

/*****************************************************************************/
#endif
