/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_DATA_PACKAGE_HH_
#define BE_DATA_PACKAGE_HH_
/*****************************************************************************/
#include    <rtti/package.hh>

namespace BugEngine { namespace Data
{

class be_api(DATA) Package : public RTTI::Package
{
private:
    ref<const RTTI::Namespace> m_namespace;
private:
    virtual void doload();
    virtual void dounload();
public:
    Package(const ipath& name);
    ~Package();

    virtual weak<const RTTI::Namespace> getNamespace() const override;

    be_metaclass(DATA, Package, RTTI::Package)
        ref<RTTI::Package> create(const ipath& name) const override;
    be_properties
    be_end
};

}}

/*****************************************************************************/
#endif
