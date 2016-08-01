/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <world/stdafx.h>
#include    <world/entitystorage.script.hh>
#include    <world/component.script.hh>
#include    <scheduler/task/method.hh>
#include    <rtti/value.hh>
#include    <componentgroup.hh>
#include    <entityinfo.hh>
#include    <cstring>

namespace BugEngine { namespace World
{

namespace
{

struct GroupInfo
{
    typedef minitl::vector< raw<const RTTI::Class> > ComponentVector;
    typedef minitl::vector< minitl::array< raw<const RTTI::Class> > > PartitionVector;
    ComponentVector components;
    PartitionVector partitions;

    GroupInfo(const minitl::array< raw<const RTTI::Class> >& initial)
        :   components(Arena::temporary(), initial.begin(), initial.end())
        ,   partitions(Arena::temporary())
    {
        partitions.push_back(initial);
    }

    template< typename IT >
    bool isCommon(IT firstComponent, IT lastComponent)
    {
        for (IT it =firstComponent; it != lastComponent; ++it)
        {
            for (ComponentVector::const_iterator it2 = components.begin();
                 it2 != components.end();
                 ++it2)
            {
                if (*it == *it2)
                {
                    return true;
                }
            }
        }
        return false;
    }

    bool tryMerge(const GroupInfo& otherGroup)
    {
        if (isCommon(otherGroup.components.begin(), otherGroup.components.end()))
        {
            for (ComponentVector::const_iterator component = otherGroup.components.begin();
                 component != otherGroup.components.end();
                 ++component)
            {
                if (find(*component, components.begin(), components.end()) == components.end())
                {
                    components.push_back(*component);
                }
            }
            for (PartitionVector::const_iterator partition = otherGroup.partitions.begin();
                 partition != otherGroup.partitions.end();
                  ++partition)
            {
                partitions.push_back(*partition);
            }

            return true;
        }
        else
        {
            return false;
        }
    }
};

}

EntityStorage::ComponentStorage::ComponentStorage(SystemAllocator& allocator, u32 componentSize)
    :   allocator(allocator)
    ,   elementCount(0)
    ,   componentSize(componentSize)
    ,   elementsPerPage(allocator.blockSize() / componentSize)
    ,   pages()
{
}

EntityStorage::ComponentStorage::~ComponentStorage()
{
    for (u32 page = 0; page < (elementCount + elementsPerPage - 1) / elementsPerPage; ++page)
    {
        allocator.free(pages[page]);
    }
}

void EntityStorage::ComponentStorage::reserve(i32 delta)
{
    if (delta > 0)
    {
        u32 oldCount = elementCount;
        elementCount += delta;
        for (u32 pageIndex = (oldCount + elementsPerPage - 1) / elementsPerPage;
             pageIndex < (elementCount + elementsPerPage - 1) / elementsPerPage;
             ++pageIndex)
        {
            pages[pageIndex] = static_cast<byte*>(allocator.allocate());
        }
    }
}

void EntityStorage::ComponentStorage::shrink(i32 delta)
{
    if (delta < 0)
    {
        u32 oldCount = elementCount;
        elementCount += delta;
        for (u32 pageIndex = 1 + (elementCount + elementsPerPage - 1) / elementsPerPage;
             pageIndex < (oldCount + elementsPerPage - 1) / elementsPerPage;
             ++pageIndex)
        {
            allocator.free(pages[pageIndex]);
        }
    }
}

byte* EntityStorage::ComponentStorage::getElement(u32 index) const
{
    u32 pageIndex = index / elementsPerPage;
    u32 offset = (index % elementsPerPage) * componentSize;
    be_assert(offset + componentSize < allocator.blockSize(),
              "component lies outside of page buffer");
    byte* page = pages[pageIndex];
    return page + offset;
}

EntityStorage::ComponentIndex::ComponentIndex()
    :   group((u16)~0)
    ,   relativeIndex((u8)~0)
    ,   absoluteIndex((u8)~0)
{
}

EntityStorage::ComponentIndex::ComponentIndex(u32 group, u32 relativeIndex, u32 absoluteIndex)
    :   group(be_checked_numcast<u16>(group))
    ,   relativeIndex(be_checked_numcast<u8>(relativeIndex))
    ,   absoluteIndex(be_checked_numcast<u8>(absoluteIndex))
{
}

EntityStorage::ComponentIndex::operator void*() const
{
    return (void*)(group != (u16)~0 || relativeIndex != (u8)~0);
}

bool EntityStorage::ComponentIndex::operator!() const
{
    return group == (u16)~0 && relativeIndex == (u8)~0;
}

EntityStorage::EntityStorage(const WorldComposition& composition)
    :   m_task(scoped< Task::Task< Task::MethodCaller<EntityStorage,
                                                      &EntityStorage::start> > >::create(
                    Arena::task(),
                    "start",
                    Colors::Green::Green,
                    Task::MethodCaller<EntityStorage, &EntityStorage::start>(this)))
    ,   m_freeEntityId(0)
    ,   m_allocator4k(SystemAllocator::BlockSize_4k, 32)
    ,   m_allocator16k(SystemAllocator::BlockSize_16k, 32)
    ,   m_allocator64k(SystemAllocator::BlockSize_64k, 32)
    ,   m_allocator256k(SystemAllocator::BlockSize_256k, 32)
    ,   m_entityInfoBuffer((u8**)m_allocator16k.allocate())
    ,   m_entityCount(0)
    ,   m_entityBufferCount(0)
    ,   m_maxEntityBufferCount(m_allocator16k.blockSize() / sizeof(EntityInfo*))
    ,   m_bufferCapacity(m_allocator256k.blockSize()
                      / (sizeof(EntityInfo) + sizeof(EntityInfo::BucketInfo) * (composition.components.size()-1)))
    ,   m_componentTypes(Arena::game(), composition.components.size())
    ,   m_componentGroups(Arena::game())
    ,   m_componentBackLinks(Arena::game())
    ,   m_components(Arena::game(), composition.components.size())
{
    m_entityInfoBuffer[0] = 0;
    buildGroups(composition);
    for (WorldComposition::ComponentArray::const_iterator it = composition.components.begin();
         it != composition.components.end();
         ++it)
    {
        ComponentIndex index = getComponentIndex(it->first);
        be_assert(index, "component %s not registered" | it->first->fullname());
        ComponentStorage* s = new (m_allocator4k.allocate())
                ComponentStorage((&m_allocator4k)[it->second], it->first->size);
        m_components[index.absoluteIndex] = s;
    }
}

EntityStorage::~EntityStorage()
{
    for (u8** buffer = m_entityInfoBuffer; *buffer != 0; ++buffer)
    {
        m_allocator256k.free(*buffer);
    }
    m_allocator16k.free(m_entityInfoBuffer);
    for (minitl::array<ComponentStorage*>::const_iterator it = m_components.begin();
         it != m_components.end();
         ++it)
    {
        (*it)->~ComponentStorage();
        m_allocator4k.free(*it);
    }
    for (minitl::vector<ComponentStorage*>::const_iterator it = m_componentBackLinks.begin();
         it != m_componentBackLinks.end();
         ++it)
    {
        (*it)->~ComponentStorage();
        m_allocator4k.free(*it);
    }
    for (minitl::vector<ComponentGroup>::iterator it = m_componentGroups.begin();
         it != m_componentGroups.end();
         ++it)
    {
        it->freeBuffers();
    }
}

weak<Task::ITask> EntityStorage::initialTask() const
{
    return m_task;
}

void EntityStorage::buildGroups(const WorldComposition& composition)
{
    minitl::vector<GroupInfo> groups(Arena::temporary(),
                                     composition.partitions.begin(),
                                     composition.partitions.end());

    if (groups.size() > 1)
    {
        for (minitl::vector<GroupInfo>::reverse_iterator it = groups.rbegin()+1;
             it != groups.rend();
             ++it)
        {
            for (minitl::vector<GroupInfo>::iterator it2 = it-1; it2 != groups.end(); /*nothing*/)
            {
                if (it->tryMerge(*it2))
                {
                    it2 = groups.erase(it2);
                }
                else
                {
                    it2 ++;
                }
            }
        }
    }

    u32 groupIndex = 0;
    u32 totalComponents = 0;
    m_componentGroups.reserve(groups.size());
    for (minitl::vector<GroupInfo>::const_iterator group = groups.begin();
         group != groups.end();
         ++group, ++groupIndex)
    {
        minitl::vector< raw<const RTTI::Class> > registeredComponents(Arena::temporary());
        u32 componentIndex = 0;
        u32 totalSize = 0;
        for (minitl::vector< raw<const RTTI::Class> >::const_iterator component = group->components.begin();
             component != group->components.end();
             ++component, ++componentIndex)
        {
            for (WorldComposition::ComponentArray::const_iterator ci = composition.components.begin();
                 ci != composition.components.end();
                 ++ci)
            {
                if (ci->first == *component)
                {
                    registeredComponents.push_back(*component);
                    registerType(*component, groupIndex, componentIndex,
                                 totalComponents + componentIndex, ci->second);
                    totalSize += (*component)->size;
                }
            }
        }
        totalComponents +=componentIndex;

        minitl::vector<u32> masks(Arena::temporary());
        for (WorldComposition::PartitionVector::const_iterator partition = group->partitions.begin();
             partition != group->partitions.end();
             ++partition)
        {
            u32 mask = buildMask(*partition);
            minitl::vector<u32> previousMasks(Arena::temporary(), masks.rbegin(), masks.rend());
            for (minitl::vector<u32>::const_iterator m = previousMasks.begin();
                 m != previousMasks.end();
                 ++m)
            {
                if (find(mask | *m, masks.begin(), masks.end()) == masks.end())
                {
                    masks.push_back(mask | *m);
                }
            }
            masks.push_back(mask);
        }

        for (u32 i = 0; i < componentIndex; ++i)
        {
            masks.push_back(1 << i);
        }
        masks.push_back(0);

        u32 componentTypeCount = be_checked_numcast<u32>(group->components.size());
        u32 componentTupeFirst = totalComponents - componentTypeCount;
        m_componentGroups.push_back(ComponentGroup(be_checked_numcast<u32>(m_componentGroups.size()),
                                                   componentTupeFirst,
                                                   group->components,
                                                   masks, m_allocator256k));
        ComponentStorage* backlink = new (m_allocator4k.allocate()) ComponentStorage(m_allocator4k,
                                                                                     sizeof(u32));
        m_componentBackLinks.push_back(backlink);
    }
}

u32 EntityStorage::buildMask(const minitl::array< raw<const RTTI::Class> >& componentList)
{
    u32 result = 0;
    for (minitl::array< raw<const RTTI::Class> >::const_iterator it = componentList.begin();
         it != componentList.end();
         ++it)
    {
        ComponentIndex index = getComponentIndex(*it);
        result |= (u32)1 << index.relativeIndex;
    }
    return result;
}

void EntityStorage::registerType(raw<const RTTI::Class> componentType, u32 group, u32 relativeIndex,
                                 u32 absoluteIndex, u32 pageSize)
{
    be_assert(!getComponentIndex(componentType),
              "component type %s is registered twice" | componentType->fullname());
    be_info("component %s: using %s kB pages" | componentType->name | 1 << (pageSize*2 + 2));
    be_assert((&m_allocator4k)[pageSize].blockSize() == 1u << (pageSize*2 + 12),
              "invalid allocator for block size %d kB" | 1 << (pageSize*2 + 2));
    ComponentInfo& info = m_componentTypes[absoluteIndex];
    info.first = componentType;
    info.second = ComponentIndex(group, relativeIndex, absoluteIndex);
    info.third = componentType->size;
}

EntityStorage::ComponentIndex EntityStorage::getComponentIndex(raw<const RTTI::Class> componentType) const
{
    for (u32 i = 0; i < m_componentTypes.size(); ++i)
    {
        if (m_componentTypes[i].first == componentType)
        {
            return m_componentTypes[i].second;
        }
    }
    return ComponentIndex();
}

const EntityStorage::ComponentInfo& EntityStorage::getComponentInfo(raw<const RTTI::Class> componentType) const
{
    for (u32 i = 0; i < m_componentTypes.size(); ++i)
    {
        if (m_componentTypes[i].first == componentType)
        {
            return m_componentTypes[i];
        }
    }
    be_notreached();
    return m_componentTypes[0];
}

void EntityStorage::start()
{
    for (minitl::vector<ComponentGroup>::iterator it = m_componentGroups.begin();
         it != m_componentGroups.end();
         ++it)
    {
        it->runEntityOperations(this);
    }
}

u32 EntityStorage::getEntityInfoSize() const
{
    return be_checked_numcast<u32>(sizeof(EntityInfo)
                                 + sizeof(EntityInfo::BucketInfo) * (m_componentTypes.size()-1));
}

EntityInfo& EntityStorage::getEntityInfo(Entity e)
{
    u32 bufferIndex = e.id >> 16;
    u32 bufferOffset = e.id & 0xFFFF;
    be_assert(bufferIndex < m_entityBufferCount,
              "Entity %d: invalid block index %d" | e.id | bufferIndex);
    be_assert(bufferOffset < m_bufferCapacity,
              "Entity %d: invalid block offset %d" | e.id | bufferOffset);
    EntityInfo& info = *(EntityInfo*)(m_entityInfoBuffer[bufferIndex] + bufferOffset*getEntityInfoSize());
    be_assert(info.index & s_usedBit,
              "Entity %d: not spawned" | e.id);
    be_assert((info.index & s_indexMask) == e.id,
              "Entity %d: inconsistent buffer" | e.id);
    return info;
}

EntityInfo& EntityStorage::getRawEntityInfo(Entity e)
{
    u32 bufferIndex = e.id >> 16;
    u32 bufferOffset = e.id & 0xFFFF;
    be_assert(bufferIndex < m_entityBufferCount,
              "Entity %d: invalid block index %d" | e.id | bufferIndex);
    be_assert(bufferOffset < m_bufferCapacity,
              "Entity %d: invalid block offset %d" | e.id | bufferOffset);
    EntityInfo& info = *(EntityInfo*)(m_entityInfoBuffer[bufferIndex] + bufferOffset*getEntityInfoSize());
    be_assert(!(info.index & s_usedBit),
              "Entity %d: already spawned" | e.id);
    be_assert((info.index & s_indexMask) != e.id,
              "Entity %d: inconsistent buffer" | e.id);
    return info;
}
const EntityInfo& EntityStorage::getEntityInfo(Entity e) const
{
    u32 bufferIndex = e.id >> 16;
    u32 bufferOffset = e.id & 0xFFFF;
    be_assert(bufferIndex < m_entityBufferCount,
              "Entity %d: invalid block index %d" | e.id | bufferIndex);
    be_assert(bufferOffset < m_bufferCapacity,
              "Entity %d: invalid block offset %d" | e.id | bufferOffset);
    const EntityInfo& info = *(const EntityInfo*)(m_entityInfoBuffer[bufferIndex] + bufferOffset*getEntityInfoSize());
    be_assert(info.index & s_usedBit,
              "Entity %d: not spawned" | e.id);
    be_assert((info.index & s_indexMask) == e.id,
              "Entity %d: inconsistent buffer" | e.id);
    return info;
}

Entity EntityStorage::spawn()
{
    Entity e = { m_freeEntityId };

    u32 bufferIndex = e.id >> 16;
    be_assert(bufferIndex <= m_entityBufferCount,
              "invalid buffer index: %d (range: %d, %d)" | bufferIndex | 0 | m_entityBufferCount);
    if (bufferIndex == m_entityBufferCount)
    {
        be_assert(m_entityInfoBuffer[bufferIndex] == 0,
                  "buffer index %d inconsistent" | bufferIndex);
        u8* buffer = (u8*)m_allocator256k.allocate();
        u32 index = (bufferIndex << 16);
        const u32 infoSize = getEntityInfoSize();
        for (u32 i = 0; i < m_bufferCapacity; ++i)
        {
            EntityInfo& info = *(EntityInfo*)(buffer + i * infoSize);
            info.next = index + i + 1;
            for (u32 j = 0; j < m_componentTypes.size(); ++j)
            {
                info.buckets[j].group = (u16)~(u16)0;
                info.buckets[j].bucket = (u16)~(u16)0;
                info.buckets[j].offset = ~(u32)0;
            }
        }
        EntityInfo& info = *(EntityInfo*)(buffer + (m_bufferCapacity-1) * infoSize);
        info.next = (1 + bufferIndex) << 16;

        m_entityInfoBuffer[bufferIndex] = buffer;
        m_entityInfoBuffer[bufferIndex+1] = 0;
        m_entityBufferCount++;
    }

    EntityInfo& entityInfo = getRawEntityInfo(e);
    be_assert((entityInfo.index & s_usedBit) == 0,
              "Entity %s is already in use; entity buffer inconsistent" | e.id);
    m_freeEntityId = entityInfo.next;
    entityInfo.index = e.id | s_usedBit;
    ++ m_entityCount;
    return e;
}

void EntityStorage::unspawn(Entity e)
{
    EntityInfo& info = getEntityInfo(e);
    // TODO: unspawn components
    info.next = m_freeEntityId;
    for (u32 i = 0; i < m_componentTypes.size(); ++i)
    {
        info.buckets[i].group = (u16)~(u16)0;
        info.buckets[i].bucket = (u16)~(u16)0;
        info.buckets[i].offset = ~(u32)0;
    }
    m_freeEntityId = e.id;
    -- m_entityCount;
}

void EntityStorage::addComponent(Entity e, const Component& component,
                                 raw<const RTTI::Class> componentType)
{
    EntityInfo& info = getEntityInfo(e);
    be_assert_recover(info.index & s_usedBit,
                      "Entity %s is not currently spawned (maybe already unspawned)" | e.id,
                      return);
    ComponentIndex componentIndex = getComponentIndex(componentType);
    be_assert_recover(componentIndex,
                      "component type %s is not a registered component of entity storage"
                            | componentType->fullname(),
                      return);

    ComponentGroup& group = m_componentGroups[componentIndex.group];
    u32 mask = 0;
    for (u32 m = 1, c = group.firstComponent;
         c < group.lastComponent;
         ++c, m <<= 1)
    {
        if (info.buckets[c].group != (u16)~0)
        {
            mask |= m;
        }
    }
    group.addComponent(e, mask, component, componentIndex.relativeIndex);
    info.buckets[componentIndex.absoluteIndex].group = componentIndex.group;
}

void EntityStorage::removeComponent(Entity e, raw<const RTTI::Class> componentType)
{
    EntityInfo& info = getEntityInfo(e);
    be_assert_recover((info.index & s_usedBit) != 0,
                      "Entity %s is not currently spawned (maybe already unspawned)" | e.id,
                      return);
    ComponentIndex componentIndex = getComponentIndex(componentType);
    be_assert_recover(componentIndex,
                      "component type %s is not a registered component of entity storage"
                            | componentType->fullname(),
                      return);
    be_assert_recover(info.buckets[componentIndex.absoluteIndex].group != (u16)~0,
                      "entity %d does not has a component %s" | e.id | componentType->fullname(),
                      return);

    ComponentGroup& group = m_componentGroups[componentIndex.group];
    u32 mask = 0;
    for (u32 m = 1, c = group.firstComponent;
         c < group.lastComponent;
         ++c, m <<= 1)
    {
        if (info.buckets[c].group != (u16)~0)
        {
            mask |= m;
        }
    }
    group.removeComponent(e, mask, componentIndex.relativeIndex);
    info.buckets[componentIndex.absoluteIndex].group = (u16)~0;
}

bool EntityStorage::hasComponent(Entity e, raw<const RTTI::Class> componentType) const
{
    const EntityInfo& info = getEntityInfo(e);
    be_assert_recover((info.index & s_usedBit) != 0,
                      "Entity %s is not currently spawned (maybe already unspawned)" | e.id,
                      return false);
    ComponentIndex componentIndex = getComponentIndex(componentType);
    be_assert_recover(componentIndex,
                      "component type %s is not a registered component of entity storage"
                            | componentType->fullname(),
                      return false);
    return info.buckets[componentIndex.absoluteIndex].group != (u16)~0;
}

void EntityStorage::copyComponent(Entity e, u32 componentAbsoluteIndex, byte target[]) const
{
    const EntityInfo& info = getEntityInfo(e);
    be_assert_recover(info.buckets[componentAbsoluteIndex].group != (u16)~0,
                      "entity %d does not use component %s"
                         | e.id | m_componentTypes[componentAbsoluteIndex].first->fullname(),
                      return);
    be_assert_recover(info.buckets[componentAbsoluteIndex].bucket != (u16)~0,
                      "entity %d has not yet spawned component %s"
                         | e.id | m_componentTypes[componentAbsoluteIndex].first->fullname(),
                      return);
    const ComponentGroup& group = m_componentGroups[info.buckets[componentAbsoluteIndex].group];
    const u32 mask = (u32)1 << (componentAbsoluteIndex - group.firstComponent);
    u32 componentOffset = info.buckets[componentAbsoluteIndex].offset;
    for (u32 bucketIndex = 0;
         bucketIndex < info.buckets[componentAbsoluteIndex].bucket;
         ++bucketIndex)
    {
        if (group.m_buckets[bucketIndex].acceptMask & mask)
        {
            componentOffset += group.m_buckets[bucketIndex].entityCount;
        }
    }
    const ComponentStorage* storage = m_components[componentAbsoluteIndex];
    memcpy(target, storage->getElement(componentOffset),
           m_componentTypes[componentAbsoluteIndex].third);
}

RTTI::Value EntityStorage::getComponent(Entity e, raw<const RTTI::Class> componentType) const
{
    be_assert(hasComponent(e, componentType),
              "Entity %s does not have a component of type %s" | e.id | componentType->fullname());
    const EntityInfo& info = getEntityInfo(e);
    const ComponentIndex componentTypeIndex = getComponentIndex(componentType);
    const ComponentGroup& group = m_componentGroups[info.buckets[componentTypeIndex.absoluteIndex].group];
    const u32 mask = (u32)1 << (componentTypeIndex.relativeIndex);
    u32 componentOffset = info.buckets[componentTypeIndex.absoluteIndex].offset;
    for (u32 bucketIndex = 0;
         bucketIndex < info.buckets[componentTypeIndex.absoluteIndex].bucket;
         ++bucketIndex)
    {
        if (group.m_buckets[bucketIndex].acceptMask & mask)
        {
            componentOffset += group.m_buckets[bucketIndex].entityCount;
        }
    }
    const ComponentStorage* storage = m_components[componentTypeIndex.absoluteIndex];
    be_assert(componentOffset < storage->elementCount,
              "component index %d out of range" | componentOffset);
    return RTTI::Value(RTTI::Type::makeType(componentType,
                                            RTTI::Type::Value,
                                            RTTI::Type::Mutable,
                                            RTTI::Type::Const),
                       storage->getElement(componentOffset));
}

ComponentGroup& EntityStorage::getComponentGroup(ComponentIndex componentIndex)
{
    be_assert(componentIndex,
              "invalid component index given to getComponentGroup");
    be_assert(componentIndex.group < m_componentGroups.size(),
              "invalid component index given to getComponentGroup");
    return m_componentGroups[componentIndex.group];
}

}}
