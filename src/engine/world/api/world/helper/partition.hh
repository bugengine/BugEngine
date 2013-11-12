/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_HELPER_PARTITION_HH_
#define BE_WORLD_HELPER_PARTITION_HH_
/*****************************************************************************/
#include    <rtti/engine/propertyinfo.script.hh>
#include    <rtti/value.hh>
#include    <world/helper/outputstream.hh>


namespace BugEngine { namespace World
{

template< typename T, typename TAIL >
struct Partition;

namespace Helper
{

template< typename T, typename T2, typename TAIL >
struct PartitionProductGetter
{
    static const Kernel::Product<T>& getProduct(const Partition<T2, TAIL>& partition)
    {
        return PartitionProductGetter<T, typename TAIL::Type, typename TAIL::Tail>::getProduct(partition);
    }
};

template< typename T, typename TAIL >
struct PartitionProductGetter<T, T, TAIL>
{
    static const Kernel::Product<T>& getProduct(const Partition<T, TAIL>& partition)
    {
        return partition.stream.product;
    }
};

}

template< typename T, typename TAIL >
struct Partition : public TAIL
{
    enum { Index = TAIL::Index+1 };
    typedef T Type;
    typedef TAIL Tail;
    const OutputStream<T> stream;
    Partition(weak<Task::ITask> task)
        :   TAIL(task)
        ,   stream(task)
    {
    }

    static const istring name();
    template< typename T2 >
    static RTTI::Value getProduct(void* from, bool isConst)
    {
        be_forceuse(isConst);
        const Partition* partition = static_cast<const Partition*>(from);
        return RTTI::Value(RTTI::Value::ByRef(Helper::PartitionProductGetter<T2, T, Tail>::getProduct(*partition)));
    }
private:
    Partition(const Partition& other);
    Partition& operator=(const Partition& other);
};


template< typename T, typename TAIL >
const istring Partition<T, TAIL>::name()
{
    return istring(minitl::format<4096u>("%s+%s") | be_typeid<T>::klass()->name | Partition<typename TAIL::Type, typename TAIL::Tail>::name());
}

template< typename T >
struct Partition<T, void>
{
    enum { Index = 0 };
    typedef T Type;
    typedef void Tail;
    const OutputStream<T> stream;
    Partition(weak<Task::ITask> task)
        :   stream(task)
    {
    }
    static const istring name();
    template< typename T2 >
    static RTTI::Value getProduct(void* from, bool isConst)
    {
        be_forceuse(isConst);
        const Partition* partition = static_cast<const Partition*>(from);
        return RTTI::Value(RTTI::Value::ByRef(Helper::PartitionProductGetter<T2, T, Tail>::getProduct(*partition)));
    }
private:
    Partition(const Partition& other);
    Partition& operator=(const Partition& other);
};


template< typename T >
const istring Partition<T, void>::name()
{
    return be_typeid<T>::klass()->name;
};

namespace Helper
{

template< typename PARTITION, typename T, typename TAIL >
struct PartitionPropertyInfo
{
    static const RTTI::Property s_property;
};

template< typename PARTITION, typename T >
struct PartitionPropertyInfo<PARTITION, T, void>
{
    static const RTTI::Property s_property;
};

template< typename PARTITION, typename T, typename TAIL >
const RTTI::Property PartitionPropertyInfo<PARTITION, T, TAIL>::s_property =
{
    {0},
    {&PartitionPropertyInfo<PARTITION, typename TAIL::Type, typename TAIL::Tail>::s_property},
    be_typeid<T>::klass()->name,
    be_typeid<PARTITION>::type(),
    be_typeid< const Kernel::Product<T>& >::type(),
    &PARTITION::template getProduct<T>
};

template< typename PARTITION, typename T >
const RTTI::Property PartitionPropertyInfo<PARTITION, T, void>::s_property =
{
    {0},
    {0},
    be_typeid<T>::klass()->name,
    be_typeid<PARTITION>::type(),
    be_typeid< const Kernel::Product<T>& >::type(),
    &PARTITION::template getProduct<T>
};

}}}


namespace BugEngine
{

template< typename T, typename TAIL >
struct be_typeid< World::Partition<T, TAIL> >
{
    static inline RTTI::Type  type()  { return RTTI::Type::makeType(preklass(), RTTI::Type::Value, RTTI::Type::Mutable, RTTI::Type::Mutable); }
    static inline raw<RTTI::Class> preklass()
    {
        static RTTI::Class s_class =
        {
            istring("Partition"),
            {0},
            be_typeid<void>::klass(),
            0,
            0,
            RTTI::ClassType_Object,
            {0},
            {&World::Helper::PartitionPropertyInfo< World::Partition<T, TAIL>, T, TAIL >::s_property},
            be_typeid<void>::klass()->methods,
            be_typeid<void>::klass()->objects,
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

#define PARTITION_1(T1)                                                                                                \
    ::BugEngine::World::Partition< T1, void >

#define PARTITION_2(T1, T2)                                                                                            \
    ::BugEngine::World::Partition< T1, PARTITION_1(T2) >

#define PARTITION_3(T1, T2, T3)                                                                                        \
    ::BugEngine::World::Partition< T1, PARTITION_2(T2, T3) >

#define PARTITION_4(T1, T2, T3, T4)                                                                                    \
    ::BugEngine::World::Partition< T1, PARTITION_3(T2, T3, T4) >

#define PARTITION_5(T1, T2, T3, T4, T5)                                                                                \
    ::BugEngine::World::Partition< T1, PARTITION_4(T2, T3, T4, T5) >

#define PARTITION_6(T1, T2, T3, T4, T5, T6)                                                                            \
    ::BugEngine::World::Partition< T1, PARTITION_5(T2, T3, T4, T5, T6) >

#define PARTITION_7(T1, T2, T3, T4, T5, T6, T7)                                                                        \
    ::BugEngine::World::Partition< T1, PARTITION_6(T2, T3, T4, T5, T6, T7) >

#define PARTITION_8(T1, T2, T3, T4, T5, T6, T7, T8)                                                                    \
    ::BugEngine::World::Partition< T1, PARTITION_7(T2, T3, T4, T5, T6, T7, T8) >

#define PARTITION_9(T1, T2, T3, T4, T5, T6, T7, T8, T9)                                                                \
    ::BugEngine::World::Partition< T1, PARTITION_8(T2, T3, T4, T5, T6, T7, T8, T9) >

#define PARTITION_10(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)                                                          \
    ::BugEngine::World::Partition< T1, PARTITION_9(T2, T3, T4, T5, T6, T7, T8, T9, T10) >

#define PARTITION_11(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)                                                     \
    ::BugEngine::World::Partition< T1, PARTITION_10(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11) >

#define PARTITION_12(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)                                                \
    ::BugEngine::World::Partition< T1, PARTITION_11(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12) >

#define PARTITION_13(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)                                           \
    ::BugEngine::World::Partition< T1, PARTITION_12(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13) >

#define PARTITION_14(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)                                      \
    ::BugEngine::World::Partition< T1, PARTITION_13(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14) >

#define PARTITION_15(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)                                 \
    ::BugEngine::World::Partition< T1, PARTITION_14(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15) >

#define PARTITION_16(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16)                            \
    ::BugEngine::World::Partition< T1, PARTITION_15(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16) \
        >

#define PARTITION_17(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17)                       \
    ::BugEngine::World::Partition< T1, PARTITION_16(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, \
        T17) >

#define PARTITION_18(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18)                  \
    ::BugEngine::World::Partition< T1, PARTITION_17(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, \
        T17, T18) >

#define PARTITION_19(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19)             \
    ::BugEngine::World::Partition< T1, PARTITION_18(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, \
        T17, T18, T19) >

#define PARTITION_20(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20)        \
    ::BugEngine::World::Partition< T1, PARTITION_19(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, \
        T17, T18, T19, T20) >

#define PARTITION_21(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21)   \
    ::BugEngine::World::Partition< T1, PARTITION_20(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, \
        T17, T18, T19, T20, T21) >

#define PARTITION_22(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21,   \
        T22)                                                                                                           \
    ::BugEngine::World::Partition< T1, PARTITION_21(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, \
        T17, T18, T19, T20, T21, T22) >

#define PARTITION_23(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21,   \
        T22, T23)                                                                                                      \
    ::BugEngine::World::Partition< T1, PARTITION_22(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, \
        T17, T18, T19, T20, T21, T22, T23) >

#define PARTITION_24(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21,   \
        T22, T23, T24)                                                                                                 \
    ::BugEngine::World::Partition< T1, PARTITION_23(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, \
        T17, T18, T19, T20, T21, T22, T23, T24) >

#define PARTITION_25(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21,   \
        T22, T23, T24, T25)                                                                                            \
    ::BugEngine::World::Partition< T1, PARTITION_24(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, \
        T17, T18, T19, T20, T21, T22, T23, T24, T25) >

#define PARTITION_26(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21,   \
        T22, T23, T24, T25, T26)                                                                                       \
    ::BugEngine::World::Partition< T1, PARTITION_25(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, \
        T17, T18, T19, T20, T21, T22, T23, T24, T25, T26) >

#define PARTITION_27(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21,   \
        T22, T23, T24, T25, T26, T27)                                                                                  \
    ::BugEngine::World::Partition< T1, PARTITION_26(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, \
        T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27) >

#define PARTITION_28(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21,   \
        T22, T23, T24, T25, T26, T27, T28)                                                                             \
    ::BugEngine::World::Partition< T1, PARTITION_27(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, \
        T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28) >

#define PARTITION_29(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21,   \
        T22, T23, T24, T25, T26, T27, T28, T29)                                                                        \
    ::BugEngine::World::Partition< T1, PARTITION_28(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, \
        T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29) >

#define PARTITION_30(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21,   \
        T22, T23, T24, T25, T26, T27, T28, T29, T30)                                                                   \
    ::BugEngine::World::Partition< T1, PARTITION_29(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, \
        T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30) >

#define PARTITION_31(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21,   \
        T22, T23, T24, T25, T26, T27, T28, T29, T30, T31)                                                              \
    ::BugEngine::World::Partition< T1, PARTITION_30(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, \
        T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31) >

#define PARTITION_32(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21,   \
        T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32)                                                         \
    ::BugEngine::World::Partition< T1, PARTITION_31(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, \
        T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32) >

/*****************************************************************************/
#endif
