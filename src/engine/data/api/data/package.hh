/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_DATA_PACKAGE_HH_
#define BE_DATA_PACKAGE_HH_
/*****************************************************************************/
#include    <rtti/package.hh>

namespace BugEngine { namespace Data
{

class DATAEXPORT Package : public RTTI::Package
{
private:
    refptr<const RTTI::Namespace> m_namespace;
private:
    virtual void doload();
    virtual void dounload();
public:
    Package(const ipath& name);
    ~Package();

    virtual const RTTI::Namespace*    getNamespace() const override;

    be_metaclass(DATAEXPORT, Package, RTTI::Package)
        refptr<RTTI::Package> create(const ipath& name) const override;
    be_properties
    be_end
};

}}

/*****************************************************************************/
#endif
