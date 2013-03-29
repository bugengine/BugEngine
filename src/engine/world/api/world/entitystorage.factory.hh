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

#define COMPONENT_LIST_1(T1)                                                   \
    ::BugEngine::World::ComponentList< T1, void >
#define COMPONENT_LIST_2(T1, T2)                                               \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_1(T2) >
#define COMPONENT_LIST_3(T1, T2, T3)                                           \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_2(T2, T3) >
#define COMPONENT_LIST_4(T1, T2, T3, T4)                                       \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_3(T2, T3, T4) >
#define COMPONENT_LIST_5(T1, T2, T3, T4, T5)                                   \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_4(T2, T3, T4, T5) >
#define COMPONENT_LIST_6(T1, T2, T3, T4, T5, T6)                               \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_5(T2, T3, T4, T5, T6)\
        >
#define COMPONENT_LIST_7(T1, T2, T3, T4, T5, T6, T7)                           \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_6(T2, T3, T4, T5, T6,\
        T7) >
#define COMPONENT_LIST_8(T1, T2, T3, T4, T5, T6, T7, T8)                       \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_7(T2, T3, T4, T5, T6,\
        T7, T8) >
#define COMPONENT_LIST_9(T1, T2, T3, T4, T5, T6, T7, T8, T9)                   \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_8(T2, T3, T4, T5, T6,\
        T7, T8, T9) >
#define COMPONENT_LIST_10(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)             \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_9(T2, T3, T4, T5, T6,\
        T7, T8, T9, T10) >
#define COMPONENT_LIST_11(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)        \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_10(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11) >
#define COMPONENT_LIST_12(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)   \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_11(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12) >
#define COMPONENT_LIST_13(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13)                                                                   \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_12(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13) >
#define COMPONENT_LIST_14(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14)                                                              \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_13(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14) >
#define COMPONENT_LIST_15(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15)                                                         \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_14(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15) >
#define COMPONENT_LIST_16(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16)                                                    \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_15(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16) >
#define COMPONENT_LIST_17(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17)                                               \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_16(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17) >
#define COMPONENT_LIST_18(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18)                                          \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_17(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18) >
#define COMPONENT_LIST_19(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19)                                     \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_18(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19) >
#define COMPONENT_LIST_20(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20)                                \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_19(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20) \
        >
#define COMPONENT_LIST_21(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21)                           \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_20(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21) >
#define COMPONENT_LIST_22(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22)                      \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_21(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22) >
#define COMPONENT_LIST_23(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23)                 \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_22(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23) >
#define COMPONENT_LIST_24(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24)            \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_23(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24) >
#define COMPONENT_LIST_25(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25)       \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_24(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25) >
#define COMPONENT_LIST_26(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26)  \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_25(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26) >
#define COMPONENT_LIST_27(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27)                                                                   \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_26(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27) >
#define COMPONENT_LIST_28(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28)                                                              \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_27(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28) >
#define COMPONENT_LIST_29(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29)                                                         \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_28(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29) >
#define COMPONENT_LIST_30(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30)                                                    \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_29(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30) >
#define COMPONENT_LIST_31(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31)                                               \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_30(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31) >
#define COMPONENT_LIST_32(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32)                                          \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_31(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32) >
#define COMPONENT_LIST_33(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33)                                     \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_32(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33) >
#define COMPONENT_LIST_34(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34)                                \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_33(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34) >
#define COMPONENT_LIST_35(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35)                           \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_34(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35) >
#define COMPONENT_LIST_36(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36)                      \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_35(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36) >
#define COMPONENT_LIST_37(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37)                 \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_36(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37) >
#define COMPONENT_LIST_38(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38)            \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_37(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38) >
#define COMPONENT_LIST_39(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39)       \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_38(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39) >
#define COMPONENT_LIST_40(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40)  \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_39(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40) >
#define COMPONENT_LIST_41(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41)                                                                   \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_40(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41) >
#define COMPONENT_LIST_42(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42)                                                              \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_41(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42) >
#define COMPONENT_LIST_43(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43)                                                         \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_42(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43) >
#define COMPONENT_LIST_44(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44)                                                    \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_43(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44) >
#define COMPONENT_LIST_45(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45)                                               \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_44(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45) >
#define COMPONENT_LIST_46(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46)                                          \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_45(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46) >
#define COMPONENT_LIST_47(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47)                                     \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_46(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47) >
#define COMPONENT_LIST_48(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48)                                \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_47(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48) >
#define COMPONENT_LIST_49(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49)                           \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_48(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49) >
#define COMPONENT_LIST_50(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50)                      \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_49(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50) >
#define COMPONENT_LIST_51(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51)                 \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_50(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51) >
#define COMPONENT_LIST_52(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52)            \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_51(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52) >
#define COMPONENT_LIST_53(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53)       \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_52(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53) >
#define COMPONENT_LIST_54(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54)  \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_53(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54) >
#define COMPONENT_LIST_55(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55)                                                                   \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_54(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55) >
#define COMPONENT_LIST_56(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56)                                                              \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_55(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56) >
#define COMPONENT_LIST_57(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57)                                                         \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_56(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57) >
#define COMPONENT_LIST_58(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58)                                                    \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_57(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58) >
#define COMPONENT_LIST_59(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59)                                               \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_58(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59) >
#define COMPONENT_LIST_60(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60)                                          \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_59(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60) >
#define COMPONENT_LIST_61(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61)                                     \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_60(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61) >
#define COMPONENT_LIST_62(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62)                                \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_61(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62) >
#define COMPONENT_LIST_63(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63)                           \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_62(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63) >
#define COMPONENT_LIST_64(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64)                      \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_63(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64) >
#define COMPONENT_LIST_65(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65)                 \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_64(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65) >
#define COMPONENT_LIST_66(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66)            \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_65(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66) >
#define COMPONENT_LIST_67(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67)       \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_66(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67) >
#define COMPONENT_LIST_68(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68)  \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_67(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67, T68) >
#define COMPONENT_LIST_69(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68,  \
        T69)                                                                   \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_68(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67, T68, T69) >
#define COMPONENT_LIST_70(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68,  \
        T69, T70)                                                              \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_69(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67, T68, T69, T70) >
#define COMPONENT_LIST_71(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68,  \
        T69, T70, T71)                                                         \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_70(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67, T68, T69, T70, T71) >
#define COMPONENT_LIST_72(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68,  \
        T69, T70, T71, T72)                                                    \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_71(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67, T68, T69, T70, T71, T72) >
#define COMPONENT_LIST_73(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68,  \
        T69, T70, T71, T72, T73)                                               \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_72(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73) >
#define COMPONENT_LIST_74(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68,  \
        T69, T70, T71, T72, T73, T74)                                          \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_73(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74) >
#define COMPONENT_LIST_75(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68,  \
        T69, T70, T71, T72, T73, T74, T75)                                     \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_74(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75) >
#define COMPONENT_LIST_76(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68,  \
        T69, T70, T71, T72, T73, T74, T75, T76)                                \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_75(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76) >
#define COMPONENT_LIST_77(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68,  \
        T69, T70, T71, T72, T73, T74, T75, T76, T77)                           \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_76(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76,  \
        T77) >
#define COMPONENT_LIST_78(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68,  \
        T69, T70, T71, T72, T73, T74, T75, T76, T77, T78)                      \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_77(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76,  \
        T77, T78) >
#define COMPONENT_LIST_79(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68,  \
        T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79)                 \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_78(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76,  \
        T77, T78, T79) >
#define COMPONENT_LIST_80(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68,  \
        T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80)            \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_79(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76,  \
        T77, T78, T79, T80) >
#define COMPONENT_LIST_81(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68,  \
        T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81)       \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_80(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76,  \
        T77, T78, T79, T80, T81) >
#define COMPONENT_LIST_82(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68,  \
        T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82)  \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_81(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76,  \
        T77, T78, T79, T80, T81, T82) >
#define COMPONENT_LIST_83(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68,  \
        T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82,  \
        T83)                                                                   \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_82(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76,  \
        T77, T78, T79, T80, T81, T82, T83) >
#define COMPONENT_LIST_84(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68,  \
        T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82,  \
        T83, T84)                                                              \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_83(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76,  \
        T77, T78, T79, T80, T81, T82, T83, T84) >
#define COMPONENT_LIST_85(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68,  \
        T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82,  \
        T83, T84, T85)                                                         \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_84(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76,  \
        T77, T78, T79, T80, T81, T82, T83, T84, T85) >
#define COMPONENT_LIST_86(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68,  \
        T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82,  \
        T83, T84, T85, T86)                                                    \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_85(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76,  \
        T77, T78, T79, T80, T81, T82, T83, T84, T85, T86) >
#define COMPONENT_LIST_87(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68,  \
        T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82,  \
        T83, T84, T85, T86, T87)                                               \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_86(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76,  \
        T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87) >
#define COMPONENT_LIST_88(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68,  \
        T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82,  \
        T83, T84, T85, T86, T87, T88)                                          \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_87(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76,  \
        T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88) >
#define COMPONENT_LIST_89(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68,  \
        T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82,  \
        T83, T84, T85, T86, T87, T88, T89)                                     \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_88(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76,  \
        T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89) >
#define COMPONENT_LIST_90(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68,  \
        T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82,  \
        T83, T84, T85, T86, T87, T88, T89, T90)                                \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_89(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76,  \
        T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90) >
#define COMPONENT_LIST_91(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68,  \
        T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82,  \
        T83, T84, T85, T86, T87, T88, T89, T90, T91)                           \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_90(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76,  \
        T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90,  \
        T91) >
#define COMPONENT_LIST_92(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68,  \
        T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82,  \
        T83, T84, T85, T86, T87, T88, T89, T90, T91, T92)                      \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_91(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76,  \
        T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90,  \
        T91, T92) >
#define COMPONENT_LIST_93(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68,  \
        T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82,  \
        T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93)                 \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_92(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76,  \
        T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90,  \
        T91, T92, T93) >
#define COMPONENT_LIST_94(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68,  \
        T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82,  \
        T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94)            \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_93(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76,  \
        T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90,  \
        T91, T92, T93, T94) >
#define COMPONENT_LIST_95(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68,  \
        T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82,  \
        T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95)       \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_94(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76,  \
        T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90,  \
        T91, T92, T93, T94, T95) >
#define COMPONENT_LIST_96(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68,  \
        T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82,  \
        T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96)  \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_95(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76,  \
        T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90,  \
        T91, T92, T93, T94, T95, T96) >
#define COMPONENT_LIST_97(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68,  \
        T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82,  \
        T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96,  \
        T97)                                                                   \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_96(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76,  \
        T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90,  \
        T91, T92, T93, T94, T95, T96, T97) >
#define COMPONENT_LIST_98(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68,  \
        T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82,  \
        T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96,  \
        T97, T98)                                                              \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_97(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76,  \
        T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90,  \
        T91, T92, T93, T94, T95, T96, T97, T98) >
#define COMPONENT_LIST_99(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,   \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68,  \
        T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82,  \
        T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96,  \
        T97, T98, T99)                                                         \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_98(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76,  \
        T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90,  \
        T91, T92, T93, T94, T95, T96, T97, T98, T99) >
#define COMPONENT_LIST_100(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12,  \
        T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26,  \
        T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,  \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54,  \
        T55, T56, T57, T58, T59, T60, T61, T62, T63, T64, T65, T66, T67, T68,  \
        T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82,  \
        T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96,  \
        T97, T98, T99, T100)                                                   \
    ::BugEngine::World::ComponentList< T1, COMPONENT_LIST_99(T2, T3, T4, T5,   \
        T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34,  \
        T35, T36, T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48,  \
        T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62,  \
        T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76,  \
        T77, T78, T79, T80, T81, T82, T83, T84, T85, T86, T87, T88, T89, T90,  \
        T91, T92, T93, T94, T95, T96, T97, T98, T99, T100) >

/*****************************************************************************/
#endif
