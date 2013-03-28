/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
 see LICENSE for detail */

#include    <package/stdafx.h>
#include    <package/nodes/package.hh>
#include    <package/nodes/zip.hh>
#include    <package/nodes/entity.hh>
#include    <package/nodes/event.hh>

#include    <rtti/value.hh>
#include    <rtti/engine/propertyinfo.script.hh>


namespace BugEngine { namespace PackageBuilder { namespace Nodes
{

ZipValue::ZipValue(weak<Package> owner)
:   m_owner(owner)
,   m_entities(Arena::packageBuilder())
,   m_events(Arena::packageBuilder())
{
}

ZipValue::~ZipValue()
{
}

void ZipValue::addEntity(ref<Entity> entity)
{
    m_entities.push_back(entity);
}

void ZipValue::addEvent(ref<Event> event)
{
    m_events.push_back(event);
}

bool ZipValue::isCompatible(const RTTI::Type& type) const
{
    be_forceuse(type);
    return false;
}

RTTI::Value ZipValue::as(const RTTI::Type& type) const
{
    be_assert(isCompatible(type), minitl::format<4096>("invalid conversion from %s to %s") | be_typeid< weak<const File> >::type() | type);
    return RTTI::Value();
}
}}}
