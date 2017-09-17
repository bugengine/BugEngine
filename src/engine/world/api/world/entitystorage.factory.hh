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
    friend struct BugEngine::be_typeid< EntityStorageFactory<COMPONENT_LIST, PARTITION_LIST> >;
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
private:
    enum
    {
        ComponentCount = 1 + COMPONENT_LIST::Index,
        PartitionCount = 1 + PARTITION_LIST::Index,
        PropertyCount = ComponentCount+PartitionCount
    };
    static raw< RTTI::staticarray<const RTTI::Property> > getProperties()
    {
        typedef RTTI::staticarray_n< PropertyCount, const RTTI::Property> PropertyArray;

        static byte s_buffer[sizeof(PropertyArray)];
        new (s_buffer) u64(PropertyCount);
        RTTI::Property* componentProps = reinterpret_cast<RTTI::Property*>(s_buffer + sizeof(u64));
        RTTI::Property* partitionProps = componentProps + ComponentCount;
        Helper::Property<EntityStorageFactory<COMPONENT_LIST, PARTITION_LIST>,
                         0,
                         typename COMPONENT_LIST::Type,
                         (StorageSize)COMPONENT_LIST::Storage,
                         typename COMPONENT_LIST::Tail>::fillProperty(componentProps);
        Helper::PartitionListPropertyInfo<EntityStorageFactory<COMPONENT_LIST, PARTITION_LIST>,
                                          0,
                                          typename PARTITION_LIST::Type,
                                          typename PARTITION_LIST::Tail>::fillProperty(partitionProps);
        PropertyArray* properties = reinterpret_cast<PropertyArray* >(s_buffer);
        raw< RTTI::staticarray<const RTTI::Property> > result = { &properties->array };
        return result;
    }
};

}}

namespace BugEngine
{

template< typename COMPONENT_LIST, typename PARTITION_LIST >
struct be_typeid< World::EntityStorageFactory<COMPONENT_LIST, PARTITION_LIST> >
{
    static inline RTTI::Type  type()
    {
        return RTTI::Type::makeType(preklass(), RTTI::Type::Value,
                                    RTTI::Type::Mutable, RTTI::Type::Mutable);
    }
    static inline raw<RTTI::Class> preklass()
    {
        static RTTI::Class s_class =
        {
            istring("EntityStorageFactory"),
            0,
            0,
            RTTI::ClassType_Object,
            {0},
            {be_typeid<World::EntityStorage>::preklass().m_ptr},
            {0},
            {&RTTI::staticarray<const RTTI::Tag>::s_null},
            {&RTTI::staticarray<const RTTI::Property>::s_null},
            {&RTTI::staticarray<const RTTI::Method>::s_null},
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
        static raw<const RTTI::Class> cls = registerProperties();
        return cls;
    }
    BE_EXPORT static raw<const RTTI::Class> s_initialisation;
private:
    static inline raw<const RTTI::Class> registerProperties()
    {
        raw<RTTI::Class> cls = preklass();
        cls->properties = World::EntityStorageFactory<COMPONENT_LIST, PARTITION_LIST>::getProperties();
        return cls;
    }
};

template< typename COMPONENT_LIST, typename PARTITION_LIST >
BE_EXPORT
raw<const RTTI::Class> be_typeid< World::EntityStorageFactory<COMPONENT_LIST, PARTITION_LIST> >::s_initialisation = klass();


}

/**************************************************************************************************/
#endif
