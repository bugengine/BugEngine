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
    mutable u32             m_propertyCount;
protected:
    Namespace(ref<ClassInfo> metaclass);
public:
    Namespace();
    ~Namespace();

    void add(const inamespace& name, const Value& value) const;
    void add(const istring& name, const Value& value) const;
    void remove(const inamespace& name) const;
    void remove(const istring& name) const;

    static weak<const Namespace> rttiRoot();

    bool empty() const { return m_propertyCount == 0; }
private:
    enum CreationPolicy { DoNotCreate, Create };
    weak<const Namespace> getNamespace(const inamespace& name, CreationPolicy policy) const;
};

}}

/*****************************************************************************/
#endif
