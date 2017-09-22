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

    static byte s_propertyBuffer[PropertyCount * sizeof(RTTI::Property)];
    static RTTI::Property* s_properties;

    static bool getProperties()
    {
        Helper::Property<STORAGE,
                         0,
                         typename COMPONENT_LIST::Type,
                         (StorageSize)COMPONENT_LIST::Storage,
                         typename COMPONENT_LIST::Tail>::fillProperty(s_properties);
        Helper::PartitionListPropertyInfo<STORAGE,
                                          0,
                                          typename PARTITION_LIST::Type,
                                          typename PARTITION_LIST::Tail>::fillProperty(s_properties + ComponentCount);
        return true;
    }
    static RTTI::Class s_class;
};

template< typename STORAGE >
byte EntityStorage_BugHelper<STORAGE>::s_propertyBuffer[PropertyCount * sizeof(RTTI::Property)];

template< typename STORAGE >
RTTI::Property* EntityStorage_BugHelper<STORAGE>::s_properties = reinterpret_cast<RTTI::Property*>(s_propertyBuffer);

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
    static BE_EXPORT istring name()
    {
        static istring s_name("EntityStorageFactory");
        return s_name;
    }
    static BE_EXPORT raw<const RTTI::Class> klass()
    {
        typedef World::EntityStorage_BugHelper<World::EntityStorageFactory<COMPONENT_LIST, PARTITION_LIST> > Helper;
        static const RTTI::Class s_class =
        {
            name(),
            0,
            0,
            RTTI::ClassType_Object,
            {0},
            {be_typeid<BugEngine::World::EntityStorage>::klass().m_ptr},
            {0},
            {0},
            { Helper::PropertyCount, Helper::s_properties },
            {0, 0},
            {0},
            {0},
            0,
            0
        };
        be_forceuse(s_propertiesSet);

        raw<const RTTI::Class> result = { &s_class };
        return result;
    }
    static BE_EXPORT bool s_propertiesSet;
};

template< typename COMPONENT_LIST, typename PARTITION_LIST >
BE_EXPORT
bool be_typeid< World::EntityStorageFactory<COMPONENT_LIST, PARTITION_LIST> >::s_propertiesSet =
            World::EntityStorage_BugHelper<World::EntityStorageFactory<COMPONENT_LIST, PARTITION_LIST> >::getProperties();

}

/**************************************************************************************************/
#endif
