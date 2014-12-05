/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_WORLD_ENTITYSTORAGE_FACTORY_HH_
#define BE_WORLD_ENTITYSTORAGE_FACTORY_HH_
/**************************************************************************************************/
#include    <world/stdafx.h>
#include    <world/entitystorage.script.hh>
#include    <rtti/engine/propertyinfo.script.hh>
#include    <rtti/typeinfo.hh>
#include    <world/helper/outputstream.hh>
#include    <world/helper/componentlist.hh>

namespace BugEngine { namespace World
{

template< typename COMPONENT_LIST, typename PARTITION_LIST >
class EntityStorageFactory : public EntityStorage
{
public:
    typedef COMPONENT_LIST FactoryComponentList;
    typedef PARTITION_LIST FactoryPartitionList;
private:
    COMPONENT_LIST m_list;
    PARTITION_LIST m_partitions;
private:
    static EntityStorage::WorldComposition createComposition()
    {
        EntityStorage::WorldComposition composition(COMPONENT_LIST::Index + 1);
        COMPONENT_LIST::addComponent(composition.components);
        composition.partitions.reserve(PARTITION_LIST::Index + 1);
        PARTITION_LIST::addPartition(composition.partitions);
        return composition;
    }
protected:
    EntityStorageFactory()
        :   EntityStorage(createComposition())
        ,   m_list(initialTask())
        ,   m_partitions(initialTask())
    {
    }
public:
    template< typename T >
    static RTTI::Value getProduct(void* from, bool isConst)
    {
        be_forceuse(isConst);
        const EntityStorageFactory* factory = static_cast<const EntityStorageFactory*>(from);
        return RTTI::Value(RTTI::Value::ByRef(Helper::ProductGetter<T, typename COMPONENT_LIST::Type, (StorageSize)COMPONENT_LIST::Storage, typename COMPONENT_LIST::Tail>::getProduct(factory->m_list)));
    }
    template< typename T >
    static RTTI::Value getPartition(void* from, bool isConst)
    {
        be_forceuse(isConst);
        const EntityStorageFactory* factory = static_cast<const EntityStorageFactory*>(from);
        return RTTI::Value(RTTI::Value::ByRef(Helper::PartitionGetter<T, typename PARTITION_LIST::Type, typename PARTITION_LIST::Tail>::getPartition(factory->m_partitions)));
    }
    static raw<const RTTI::Property> s_properties;
};

template< typename COMPONENT_LIST, typename PARTITION_LIST >
raw<const RTTI::Property> EntityStorageFactory<COMPONENT_LIST, PARTITION_LIST>::s_properties =
{
    &Helper::ComponentListPropertyInfo<EntityStorageFactory<COMPONENT_LIST, PARTITION_LIST>,
                                       typename COMPONENT_LIST::Type,
                                       (StorageSize)COMPONENT_LIST::Storage,
                                       typename COMPONENT_LIST::Tail>::s_property
};

}}

namespace BugEngine
{

template< typename COMPONENT_LIST, typename PARTITION_LIST >
struct be_typeid< World::EntityStorageFactory<COMPONENT_LIST, PARTITION_LIST> >
{
    static inline RTTI::Type  type()  { return RTTI::Type::makeType(preklass(), RTTI::Type::Value, RTTI::Type::Mutable, RTTI::Type::Mutable); }
    static inline raw<RTTI::Class> preklass()
    {
        static RTTI::Class s_class =
        {
            istring("EntityStorageFactory"),
            {0},
            be_typeid<World::EntityStorage>::klass(),
            0,
            0,
            RTTI::ClassType_Object,
            {0},
            World::EntityStorageFactory<COMPONENT_LIST, PARTITION_LIST>::s_properties,
            be_typeid<World::EntityStorage>::klass()->methods,
            be_typeid<World::EntityStorage>::klass()->objects,
            {0},
            {0},
            0,
            0
        };
        raw<RTTI::Class> result = { &s_class };
        return result;
    }
    static inline raw<const RTTI::Class> klass()
    {
        return preklass();
    }
};

}

/**************************************************************************************************/
#endif
