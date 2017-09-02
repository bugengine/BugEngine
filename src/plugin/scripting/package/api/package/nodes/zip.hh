/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
 see LICENSE for detail */

#ifndef BE_PACKAGE_NODES_ZIP_HH_
#define BE_PACKAGE_NODES_ZIP_HH_
/**************************************************************************************************/
#include    <package/stdafx.h>
#include    <package/nodes/value.hh>
#include    <package/nodes/entity.hh>


namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

class Package;
class Entity;
class Event;

class ZipValue : public Value
{
    friend class Package;
private:
    weak< Package > const           m_owner;
    minitl::vector< ref<Entity> >   m_entities;
    minitl::vector< ref<Event> >    m_events;
public:
    ZipValue(weak<Package> owner);
    ~ZipValue();

    void addEntity(ref<Entity> entity);
    void addEvent(ref<Event> event);

    virtual RTTI::Type::ConversionCost calculateConversion(const RTTI::Type& type) const override;
    virtual RTTI::Value as(const RTTI::Type& type) const override;

};

}}}

/**************************************************************************************************/
#endif
