/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
 see LICENSE for detail */

#include <bugengine/plugin.scripting.package/stdafx.h>
#include <bugengine/plugin.scripting.package/nodes/entity.hh>
#include <bugengine/plugin.scripting.package/nodes/event.hh>
#include <bugengine/plugin.scripting.package/nodes/package.hh>
#include <bugengine/plugin.scripting.package/nodes/zip.hh>

#include <bugengine/rtti/engine/propertyinfo.script.hh>
#include <bugengine/rtti/value.hh>

namespace BugEngine { namespace PackageBuilder { namespace Nodes {

ZipValue::ZipValue(weak< Package > owner)
    : m_owner(owner)
    , m_entities(Arena::packageBuilder())
    , m_events(Arena::packageBuilder())
{
}

ZipValue::~ZipValue()
{
}

void ZipValue::addEntity(ref< Entity > entity)
{
    m_entities.push_back(entity);
}

void ZipValue::addEvent(ref< Event > event)
{
    m_events.push_back(event);
}

RTTI::ConversionCost ZipValue::calculateConversion(const RTTI::Type& type) const
{
    be_forceuse(type);
    return RTTI::ConversionCost::s_incompatible;
}

RTTI::Value ZipValue::as(const RTTI::Type& type) const
{
    be_assert(calculateConversion(type) < RTTI::ConversionCost::s_incompatible,
              "invalid conversion from %s to %s" | be_type< weak< const File > >() | type);
    return RTTI::Value();
}
}}}  // namespace BugEngine::PackageBuilder::Nodes
