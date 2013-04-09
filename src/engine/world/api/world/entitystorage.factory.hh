/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_ENTITYSTORAGE_FACTORY_HH_
#define BE_WORLD_ENTITYSTORAGE_FACTORY_HH_
/*****************************************************************************/
#include    <world/entitystorage.script.hh>
#include    <rtti/engine/propertyinfo.script.hh>
#include    <rtti/typeinfo.hh>
#include    <world/helper/outputstream.hh>
#include    <world/helper/componentlist.hh>
#include    <world/helper/componentbitset.hh>

namespace BugEngine { namespace World
{

template< typename COMPONENT_LIST >
class EntityStorageFactory : public EntityStorage
{
    COMPONENT_LIST m_list;
protected:
    EntityStorageFactory()
        :   EntityStorage()
        ,   m_list(initialTask())
    {
        registerTypes(m_list);
    }
    template< typename T, typename TAIL >
    void registerTypes(const ComponentList<T, TAIL>& list)
    {
        registerType(be_typeid<T>::klass());
        registerTypes(static_cast<const ComponentList<typename TAIL::Type, typename TAIL::Tail>&>(list));
    }
    template< typename T >
    void registerTypes(const ComponentList<T, void>& /*list*/)
    {
        registerType(be_typeid<T>::klass());
    }
public:
    template< typename T >
    static RTTI::Value getProduct(void* from, bool isConst)
    {
        be_forceuse(isConst);
        const EntityStorageFactory* factory = static_cast<const EntityStorageFactory*>(from);
        return RTTI::Value(RTTI::Value::ByRef(Helper::ProductGetter<T, typename COMPONENT_LIST::Type, typename COMPONENT_LIST::Tail>::getProduct(factory->m_list)));
    }
    static raw<const RTTI::Property> s_properties;
};

template< typename COMPONENT_LIST >
raw<const RTTI::Property> EntityStorageFactory<COMPONENT_LIST>::s_properties = { &Helper::PropertyInfo<EntityStorageFactory<COMPONENT_LIST>, typename COMPONENT_LIST::Type, typename COMPONENT_LIST::Tail>::s_property };

}}

namespace BugEngine
{

template< typename COMPONENT_LIST >
struct be_typeid< World::EntityStorageFactory<COMPONENT_LIST> >
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
            World::EntityStorageFactory<COMPONENT_LIST>::s_properties,
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

/*****************************************************************************/
#endif
