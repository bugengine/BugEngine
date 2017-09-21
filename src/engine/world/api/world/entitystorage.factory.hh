/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_WORLD_ENTITYSTORAGE_FACTORY_HH_
#define BE_WORLD_ENTITYSTORAGE_FACTORY_HH_
/**************************************************************************************************/
#include    <world/stdafx.h>
#include    <world/entitystorage.script.hh>
#include    <rtti/engine/propertyinfo.script.hh>
#include    <rtti/typeinfo.hh>
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
        return RTTI::Value(RTTI::Value::ByRef(Helper::ProductGetter<
                                              T,
                                              typename COMPONENT_LIST::Type,
                                              (StorageSize)COMPONENT_LIST::Storage,
                                              typename COMPONENT_LIST::Tail>::getProduct(factory->m_list)));
    }
    template< typename T >
    static RTTI::Value getPartition(void* from, bool isConst)
    {
        be_forceuse(isConst);
        const EntityStorageFactory* factory = static_cast<const EntityStorageFactory*>(from);
        return RTTI::Value(RTTI::Value::ByRef(Helper::PartitionGetter<
                                              T,
                                              typename PARTITION_LIST::Type,
                                              typename PARTITION_LIST::Tail>::getPartition(factory->m_partitions)));
    }
};

template< typename STORAGE >
struct EntityStorage_BugHelper
{
    typedef typename STORAGE::FactoryComponentList COMPONENT_LIST;
    typedef typename STORAGE::FactoryPartitionList PARTITION_LIST;
    enum
    {
        ComponentCount = 1 + COMPONENT_LIST::Index,
        PartitionCount = 1 + PARTITION_LIST::Index,
        PropertyCount = ComponentCount+PartitionCount
    };
    static RTTI::staticarray<const RTTI::Property> getProperties()
    {
        static byte s_propertyBuffer[PropertyCount * sizeof(RTTI::Property)];
        RTTI::Property* properties = reinterpret_cast<RTTI::Property*>(s_propertyBuffer);

        Helper::Property<STORAGE,
                         0,
                         typename COMPONENT_LIST::Type,
                         (StorageSize)COMPONENT_LIST::Storage,
                         typename COMPONENT_LIST::Tail>::fillProperty(properties);
        Helper::PartitionListPropertyInfo<STORAGE,
                                          0,
                                          typename PARTITION_LIST::Type,
                                          typename PARTITION_LIST::Tail>::fillProperty(properties + ComponentCount);
        RTTI::staticarray<const RTTI::Property> result = { PropertyCount, properties };
        return result;
    }
    static RTTI::Class s_class;
};

template< typename STORAGE >
RTTI::Class EntityStorage_BugHelper<STORAGE>::s_class =
{
    istring("EntityStorageFactory"),
    0,
    0,
    RTTI::ClassType_Object,
    {0},
    {be_typeid<BugEngine::World::EntityStorage>::klass().m_ptr},
    {0},
    {0},
    getProperties(),
    {0, 0},
    {0},
    {0},
    0,
    0
};

}}

namespace BugEngine
{

template< typename COMPONENT_LIST, typename PARTITION_LIST >
struct be_typeid< World::EntityStorageFactory<COMPONENT_LIST, PARTITION_LIST> >
{
    static inline RTTI::Type  type()
    {
        return RTTI::Type::makeType(klass(), RTTI::Type::Value,
                                    RTTI::Type::Mutable, RTTI::Type::Mutable);
    }
    static BE_EXPORT raw<RTTI::Class> ns()
    {
        raw<RTTI::Class> result = { &World::EntityStorage_BugHelper<World::EntityStorageFactory<COMPONENT_LIST, PARTITION_LIST> >::s_class };
        return result;
    }
    static BE_EXPORT raw<const RTTI::Class> klass()
    {
        return ns();
    }
};

}

/**************************************************************************************************/
#endif
