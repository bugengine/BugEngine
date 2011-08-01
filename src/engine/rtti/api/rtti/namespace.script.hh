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

be_meta(Namespace::MetaClassInfo)
class be_api(RTTI) Namespace : public minitl::refcountable
{
    friend class ::BugEngine::Value;
    BE_NOCOPY(Namespace);
published:
    class MetaClassInfo;
    mutable minitl::hashmap<istring, Value> decls;
published:
    Namespace();
    ~Namespace();

published:
    void add(const inamespace& name, const Value& value) const;
    void add(const istring& name, const Value& value) const;
    void remove(const inamespace& name) const;
    void remove(const istring& name) const;
    Value get(const istring& name) const;

    static weak<const Namespace> rttiRoot();

    bool empty() const { return decls.empty(); }
private:
    enum CreationPolicy { DoNotCreate, Create };
    weak<const Namespace> getNamespace(const inamespace& name, CreationPolicy policy) const;
};

}}

/*****************************************************************************/
#endif
