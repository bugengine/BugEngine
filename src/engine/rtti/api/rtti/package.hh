/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_PACKAGE_HH_
#define BE_RTTI_PACKAGE_HH_
/*****************************************************************************/
#include    <rtti/helper.hh>
#include    <core/utils/singleton.hh>

namespace BugEngine { namespace RTTI
{

class Namespace;

class RTTIEXPORT Package : public Object
{
protected:
    size_t                                  m_refcount;
    istring                                 m_name;
    refptr<RTTI::Namespace>                 m_namespace;
    Package*                                m_parent;
    minitl::map< istring, refptr<Package> > m_children;
private:
    virtual void doload() = 0;
    virtual void dounload() = 0;
private:
    Package();
public:
    Package(const istring& name, Package* parent);
    ~Package();

    void load();
    void unload();

    virtual const Namespace*    getNamespace() const = 0;

    const istring& name() const { return m_name; }

    static Package* get(const inamespace& name);

    be_metaclass(RTTIEXPORT,Package,Object)
        virtual refptr<Package> create(const ipath& name) const;
    be_properties
    be_end
};

}}

/*****************************************************************************/
#endif
