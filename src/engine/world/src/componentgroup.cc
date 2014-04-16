/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <world/stdafx.h>
#include    <componentgroup.hh>
#include    <world/entitystorage.script.hh>
#include    <world/component.script.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/value.hh>
#include    <rtti/typeinfo.hh>
#include    <minitl/algorithm.hh>
#include    <entityinfo.hh>

namespace BugEngine { namespace World
{

namespace
{

static raw<const RTTI::Method::Overload> getMethodFromClass(raw<const RTTI::Class> type, istring name)
{
    raw<const RTTI::Method> method = type->getMethod(name);
    be_assert_recover(method,
                      "could not locate method \"%s\" for component %s" | name | type->fullname(),
                      return raw<const RTTI::Method::Overload>());
    for (raw<const RTTI::Method::Overload> overload = method->overloads; overload; overload = overload->next)
    {
        RTTI::Type t = RTTI::Type::makeType(type, RTTI::Type::Value, RTTI::Type::Mutable, RTTI::Type::Mutable);
        if (overload->parameterCount == 1 && t.isA(overload->params->type))
        {
            return overload;
        }
    }
    return raw<const RTTI::Method::Overload>();
}

}

ComponentGroup::ComponentGroup(u32 firstComponent,
                               const minitl::vector< raw<const RTTI::Class> >& componentTypes,
                               const minitl::vector<u32>& bucketMasks,
                               SystemAllocator& allocator)
    :   m_allocator(allocator)
    ,   m_buckets(Arena::game(), (u32)bucketMasks.size())
    ,   m_components(Arena::game(), (u32)componentTypes.size())
    ,   m_componentsTotalSize(0)
    ,   m_componentCounts((u32*)Arena::game().alloc(sizeof(u32)*componentTypes.size()*bucketMasks.size()))
    ,   firstComponent(firstComponent)
    ,   lastComponent(firstComponent + (u32)componentTypes.size())
{
    u32 index = 0;
    u32 count = (u32)componentTypes.size();
    memset(m_componentCounts, 0, sizeof(u32)*count*bucketMasks.size());
    for (minitl::vector<u32>::const_iterator it = bucketMasks.begin();
         it != bucketMasks.end();
         ++it, ++index)
    {
        m_buckets[index] = Bucket(m_componentCounts + count*index, *it);
    }
    minitl::vector< raw<const RTTI::Class> >::const_iterator begin = componentTypes.begin();
    for (minitl::vector< raw<const RTTI::Class> >::const_iterator it = begin;
        it != componentTypes.end();
        ++it)
    {
        ComponentInfo& info = m_components[be_checked_numcast<u32>(minitl::distance(begin, it))];
        info.componentType = *it;
        info.created = getMethodFromClass(*it, "created");
        if (!info.created)
        {
            be_error("component type %s: invalid signature for method \"created\""
                        | (*it)->fullname());
        }
        info.destroyed = getMethodFromClass(*it, "destroyed");
        if (!info.destroyed)
        {
            be_error("component type %s: invalid signature for method \"destroyed\""
                        | (*it)->fullname());
        }
        info.size = (*it)->size;

        m_componentsTotalSize += (*it)->size;
    }
}

ComponentGroup::~ComponentGroup()
{
}

ComponentGroup::BucketPair ComponentGroup::findBuckets(u32 mask1, u32 mask2)
{
    BucketPair result;
    minitl::tuple<u32, u32> best;
    for (Bucket* bucket = m_buckets.begin(); bucket != m_buckets.end(); ++bucket)
    {
        if ((bucket->acceptMask & mask1) == bucket->acceptMask && bucket->maskSize >= best.first)
        {
            result.first = bucket;
            best.first = bucket->maskSize;
        }
        if ((bucket->acceptMask & mask2) == bucket->acceptMask && bucket->maskSize >= best.second)
        {
            result.second = bucket;
            best.second = bucket->maskSize;
        }
    }
    return result;
}

void ComponentGroup::runEntityOperations(weak<EntityStorage> storage)
{
    be_forceuse(storage);
}

void ComponentGroup::freeBuffers()
{
    Arena::game().free(m_componentCounts);
}

void ComponentGroup::addComponent(Entity e, u32 originalMask,
                                  const Component& c, u32 componentIndex)
{
    be_forceuse(e);
    be_forceuse(originalMask);
    be_forceuse(c);
    be_forceuse(componentIndex);
}

void ComponentGroup::removeComponent(Entity e, u32 originalMask, u32 componentIndex)
{
    be_forceuse(e);
    be_forceuse(originalMask);
    be_forceuse(componentIndex);
}


}}
