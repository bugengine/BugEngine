/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_PACKAGE_HH_
#define BE_RTTI_PACKAGE_HH_
/*****************************************************************************/
#include    <rtti/helper.hh>

namespace BugEngine { namespace RTTI
{

class Namespace;

class be_api(RTTI) Package : public Object
{
protected:
    size_t                                  m_refcount;
    istring                                 m_name;
    ref<RTTI::Namespace>                    m_namespace;
    weak<Package>                           m_parent;
    minitl::map< istring, ref<Package> >    m_children;
private:
    virtual void doload() = 0;
    virtual void dounload() = 0;
private:
    Package();
public:
    Package(const istring& name, weak<Package> parent);
    ~Package();

    void load();
    void unload();

    virtual weak<const Namespace>    getNamespace() const = 0;

    const istring& name() const { return m_name; }

    static weak<Package> get(const inamespace& name);

    be_metaclass(RTTI,Package,Object)
        virtual ref<Package> create(const ipath& name) const;
    be_properties
    be_end
};

}}

/*****************************************************************************/
#endif
