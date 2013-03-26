/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_ENTITYSTORAGE_FACTORY_HH_
#define BE_WORLD_ENTITYSTORAGE_FACTORY_HH_
/*****************************************************************************/
#include    <world/entitystorage.script.hh>
#include    <minitl/typemanipulation.hh>
#include    <rtti/engine/propertyinfo.script.hh>
#include    <rtti/typeinfo.hh>
#include    <rtti/value.hh>
#include    <scheduler/kernel/product.hh>

namespace BugEngine { namespace World
{

template< typename T >
struct OutputStream
{
    scoped< Kernel::Stream<T> > stream;
    const Kernel::Product<T>    product;
    OutputStream(weak<Task::ITask> producer)
        :   stream(scoped< Kernel::Stream<T> >::create(Arena::task()))
        ,   product(stream, producer)
    {
    }
private:
    OutputStream(const OutputStream& other);
    OutputStream& operator=(const OutputStream& other);
};

template< typename T, typename TAIL >
struct ComponentList : public TAIL
{
    typedef T Type;
    typedef TAIL Tail;
    const OutputStream<T> stream;
    ComponentList(weak<Task::ITask> task)
        :   TAIL(task)
        ,   stream(task)
    {
    }
private:
    ComponentList(const ComponentList& other);
    ComponentList& operator=(const ComponentList& other);
};

template< typename T >
struct ComponentList<T, void>
{
    typedef T Type;
    typedef void Tail;
    const OutputStream<T> stream;
    ComponentList(weak<Task::ITask> task)
        :   stream(task)
    {
    }
};


template< typename T, typename T2, typename TAIL >
struct ProductGetter
{
    static const Kernel::Product<T>& getProduct(const ComponentList<T2, TAIL>& list)
    {
        return ProductGetter<T, typename TAIL::Type, typename TAIL::Tail>::getProduct(list);
    }
};

template< typename T, typename TAIL >
struct ProductGetter<T, T, TAIL>
{
    static const Kernel::Product<T>& getProduct(const ComponentList<T, TAIL>& list)
    {
        return list.stream.product;
    }
};

template< typename LIST, typename T = typename LIST::Type, typename TAIL = typename LIST::Tail >
struct PropertyInfo
{
    static const RTTI::Property s_property;
};

template< typename LIST, typename T >
struct PropertyInfo<LIST, T, void>
{
    static const RTTI::Property s_property;
};

template< typename LIST, typename T, typename TAIL >
const RTTI::Property PropertyInfo<LIST, T, TAIL>::s_property =
{
    {0},
    {&PropertyInfo<LIST, typename TAIL::Type, typename TAIL::Tail>::s_property},
    be_typeid<T>::klass()->name,
    be_typeid<LIST>::type(),
    be_typeid< const Kernel::Product<T>& >::type(),
    &LIST::template getProduct<T>
};


template< typename LIST, typename T >
const RTTI::Property PropertyInfo<LIST, T, void>::s_property =
{
    {0},
    be_typeid<EntityStorage>::klass()->properties,
    be_typeid<T>::klass()->name,
    be_typeid<LIST>::type(),
    be_typeid< const Kernel::Product<T>& >::type(),
    &LIST::template getProduct<T>
};


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
        return RTTI::Value(RTTI::Value::ByRef(ProductGetter<T, typename COMPONENT_LIST::Type, typename COMPONENT_LIST::Tail>::getProduct(factory->m_list)));
    }
    static raw<const RTTI::Property> s_properties;
};

template< typename COMPONENT_LIST >
raw<const RTTI::Property> EntityStorageFactory<COMPONENT_LIST>::s_properties = { &PropertyInfo<EntityStorageFactory<COMPONENT_LIST>, typename COMPONENT_LIST::Type, typename COMPONENT_LIST::Tail>::s_property };

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

#define COMPONENT_LIST_1(T1)                                                                        \
    ::BugEngine::World::ComponentList<T1, void>
#define COMPONENT_LIST_2(T1, T2)                                                                    \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_1(T2) >
#define COMPONENT_LIST_3(T1, T2, T3)                                                                \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_2(T2, T3) >
#define COMPONENT_LIST_4(T1, T2, T3, T4)                                                            \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_3(T2, T3, T4) >

/*****************************************************************************/
#endif
