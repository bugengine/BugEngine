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
    static raw< RTTI::staticarray<const RTTI::Property> > getProperties()
    {
        static byte s_buffer[sizeof(RTTI::staticarray_n< 1+COMPONENT_LIST::Index, RTTI::Property>)];
        RTTI::staticarray_n< 1+COMPONENT_LIST::Index, RTTI::Property>* properties =
            reinterpret_cast<RTTI::staticarray_n< 1+COMPONENT_LIST::Index, RTTI::Property>* >(s_buffer);
        new (properties) u32(1+COMPONENT_LIST::Index);
        Helper::Property<EntityStorageFactory<COMPONENT_LIST, PARTITION_LIST>,
                         COMPONENT_LIST::Index,
                         typename COMPONENT_LIST::Type,
                         (StorageSize)COMPONENT_LIST::Storage,
                         typename COMPONENT_LIST::Tail>::fillProperty(properties->elements);
        raw< RTTI::staticarray<const RTTI::Property> > result = {
                reinterpret_cast< RTTI::staticarray<const RTTI::Property>* >(properties)
            };
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
            {RTTI::staticarray<const RTTI::Tag>::s_null},
            {RTTI::staticarray<const RTTI::Property>::s_null},
            {RTTI::staticarray<const RTTI::Method>::s_null},
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
private:
    static inline raw<const RTTI::Class> registerProperties()
    {
        raw<RTTI::Class> cls = preklass();
        cls->properties = World::EntityStorageFactory<COMPONENT_LIST, PARTITION_LIST>::getProperties();
        return cls;
    }
};


}

/**************************************************************************************************/
#endif
