/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_HELPER_PARTITIONLIST_HH_
#define BE_WORLD_HELPER_PARTITIONLIST_HH_
/*****************************************************************************/
#include    <rtti/engine/propertyinfo.script.hh>
#include    <rtti/value.hh>
#include    <world/helper/outputstream.hh>
#include    <world/helper/partition.hh>


namespace BugEngine { namespace World
{

template< typename T, typename TAIL >
struct PartitionList : public TAIL
{
    enum { Index = TAIL::Index+1 };
    typedef T Type;
    typedef TAIL Tail;
    T m_partition;
    PartitionList()
        :   TAIL()
        ,   m_partition()
    {
    }
private:
    PartitionList(const PartitionList& other);
    PartitionList& operator=(const PartitionList& other);
};

template< typename T >
struct PartitionList<T, void>
{
    enum { Index = 0 };
    typedef T Type;
    typedef void Tail;
    PartitionList()
    {
    }
private:
    PartitionList(const PartitionList& other);
    PartitionList& operator=(const PartitionList& other);
};

namespace Helper
{

template< typename LIST, typename T, typename TAIL >
struct PartitionListPropertyInfo
{
    static const RTTI::Property s_property;
};

template< typename LIST, typename T >
struct PartitionListPropertyInfo<LIST, T, void>
{
    static const RTTI::Property s_property;
};

template< typename LIST, typename T, typename TAIL >
const RTTI::Property PartitionListPropertyInfo<LIST, T, TAIL>::s_property =
{
    {0},
    {&PartitionListPropertyInfo<LIST, typename TAIL::Type, typename TAIL::Tail>::s_property},
    T::name(),
    be_typeid<LIST>::type(),
    be_typeid< const T& >::type(),
    0
};

template< typename LIST, typename T >
const RTTI::Property PartitionListPropertyInfo<LIST, T, void>::s_property =
{
    {0},
    be_typeid<EntityStorage>::klass()->properties,
    T::name(),
    be_typeid<LIST>::type(),
    be_typeid< const T& >::type(),
    0
};

}}}



#define PARTITION_LIST_1(T1)                                                                                           \
    ::BugEngine::World::PartitionList< T1, void >

#define PARTITION_LIST_2(T1, T2)                                                                                       \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_1(T2) >

#define PARTITION_LIST_3(T1, T2, T3)                                                                                   \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_2(T2, T3) >

#define PARTITION_LIST_4(T1, T2, T3, T4)                                                                               \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_3(T2, T3, T4) >

#define PARTITION_LIST_5(T1, T2, T3, T4, T5)                                                                           \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_4(T2, T3, T4, T5) >

#define PARTITION_LIST_6(T1, T2, T3, T4, T5, T6)                                                                       \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_5(T2, T3, T4, T5, T6) >

#define PARTITION_LIST_7(T1, T2, T3, T4, T5, T6, T7)                                                                   \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_6(T2, T3, T4, T5, T6, T7) >

#define PARTITION_LIST_8(T1, T2, T3, T4, T5, T6, T7, T8)                                                               \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_7(T2, T3, T4, T5, T6, T7, T8) >

#define PARTITION_LIST_9(T1, T2, T3, T4, T5, T6, T7, T8, T9)                                                           \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_8(T2, T3, T4, T5, T6, T7, T8, T9) >

#define PARTITION_LIST_10(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)                                                     \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_9(T2, T3, T4, T5, T6, T7, T8, T9, T10) >

#define PARTITION_LIST_11(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)                                                \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_10(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11) >

#define PARTITION_LIST_12(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)                                           \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_11(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12) >

#define PARTITION_LIST_13(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)                                      \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_12(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13) >

#define PARTITION_LIST_14(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)                                 \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_13(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)  \
        >

#define PARTITION_LIST_15(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)                            \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_14(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15) >

#define PARTITION_LIST_16(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16)                       \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_15(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16) >

#define PARTITION_LIST_17(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17)                  \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_16(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17) >

#define PARTITION_LIST_18(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18)             \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_17(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18) >

#define PARTITION_LIST_19(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19)        \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_18(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19) >

#define PARTITION_LIST_20(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20)   \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_19(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20) >

#define PARTITION_LIST_21(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21)                                                                                                           \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_20(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21) >

#define PARTITION_LIST_22(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22)                                                                                                      \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_21(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22) >

#define PARTITION_LIST_23(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23)                                                                                                 \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_22(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23) >

#define PARTITION_LIST_24(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24)                                                                                            \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_23(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24) >

#define PARTITION_LIST_25(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25)                                                                                       \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_24(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25) >

#define PARTITION_LIST_26(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26)                                                                                  \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_25(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26) >

#define PARTITION_LIST_27(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27)                                                                             \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_26(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27) >

#define PARTITION_LIST_28(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28)                                                                        \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_27(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28) >

#define PARTITION_LIST_29(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29)                                                                   \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_28(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29) >

#define PARTITION_LIST_30(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30)                                                              \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_29(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30) >

#define PARTITION_LIST_31(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31)                                                         \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_30(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31) >

#define PARTITION_LIST_32(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32)                                                    \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_31(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32) >

#define PARTITION_LIST_1(T1)                                                                                           \
    ::BugEngine::World::PartitionList< T1, void >

#define PARTITION_LIST_2(T1, T2)                                                                                       \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_1(T2) >

#define PARTITION_LIST_3(T1, T2, T3)                                                                                   \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_2(T2, T3) >

#define PARTITION_LIST_4(T1, T2, T3, T4)                                                                               \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_3(T2, T3, T4) >

#define PARTITION_LIST_5(T1, T2, T3, T4, T5)                                                                           \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_4(T2, T3, T4, T5) >

#define PARTITION_LIST_6(T1, T2, T3, T4, T5, T6)                                                                       \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_5(T2, T3, T4, T5, T6) >

#define PARTITION_LIST_7(T1, T2, T3, T4, T5, T6, T7)                                                                   \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_6(T2, T3, T4, T5, T6, T7) >

#define PARTITION_LIST_8(T1, T2, T3, T4, T5, T6, T7, T8)                                                               \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_7(T2, T3, T4, T5, T6, T7, T8) >

#define PARTITION_LIST_9(T1, T2, T3, T4, T5, T6, T7, T8, T9)                                                           \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_8(T2, T3, T4, T5, T6, T7, T8, T9) >

#define PARTITION_LIST_10(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)                                                     \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_9(T2, T3, T4, T5, T6, T7, T8, T9, T10) >

#define PARTITION_LIST_11(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)                                                \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_10(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11) >

#define PARTITION_LIST_12(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)                                           \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_11(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12) >

#define PARTITION_LIST_13(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)                                      \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_12(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13) >

#define PARTITION_LIST_14(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)                                 \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_13(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)  \
        >

#define PARTITION_LIST_15(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)                            \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_14(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15) >

#define PARTITION_LIST_16(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16)                       \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_15(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16) >

#define PARTITION_LIST_17(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17)                  \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_16(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17) >

#define PARTITION_LIST_18(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18)             \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_17(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18) >

#define PARTITION_LIST_19(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19)        \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_18(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19) >

#define PARTITION_LIST_20(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20)   \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_19(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20) >

#define PARTITION_LIST_21(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21)                                                                                                           \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_20(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21) >

#define PARTITION_LIST_22(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22)                                                                                                      \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_21(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22) >

#define PARTITION_LIST_23(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23)                                                                                                 \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_22(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23) >

#define PARTITION_LIST_24(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24)                                                                                            \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_23(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24) >

#define PARTITION_LIST_25(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25)                                                                                       \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_24(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25) >

#define PARTITION_LIST_26(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26)                                                                                  \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_25(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26) >

#define PARTITION_LIST_27(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27)                                                                             \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_26(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27) >

#define PARTITION_LIST_28(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28)                                                                        \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_27(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28) >

#define PARTITION_LIST_29(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29)                                                                   \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_28(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29) >

#define PARTITION_LIST_30(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30)                                                              \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_29(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30) >

#define PARTITION_LIST_31(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31)                                                         \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_30(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31) >

#define PARTITION_LIST_32(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32)                                                    \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_31(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32) >

#define PARTITION_LIST_33(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33)                                               \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_32(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33) >

#define PARTITION_LIST_34(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34)                                          \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_33(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34) >

#define PARTITION_LIST_35(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35)                                     \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_34(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35) >

#define PARTITION_LIST_36(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36)                                \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_35(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36)  \
        >

#define PARTITION_LIST_37(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37)                           \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_36(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37) >

#define PARTITION_LIST_38(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38)                      \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_37(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38) >

#define PARTITION_LIST_39(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39)                 \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_38(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39) >

#define PARTITION_LIST_40(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40)            \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_39(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40) >

#define PARTITION_LIST_41(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41)       \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_40(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41) >

#define PARTITION_LIST_42(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42)  \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_41(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42) >

#define PARTITION_LIST_43(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43)                                                                                                           \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_42(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43) >

#define PARTITION_LIST_44(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44)                                                                                                      \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_43(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44) >

#define PARTITION_LIST_45(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45)                                                                                                 \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_44(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45) >

#define PARTITION_LIST_46(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46)                                                                                            \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_45(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46) >

#define PARTITION_LIST_47(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47)                                                                                       \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_46(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47) >

#define PARTITION_LIST_48(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48)                                                                                  \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_47(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48) >

#define PARTITION_LIST_49(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49)                                                                             \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_48(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49) >

#define PARTITION_LIST_50(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50)                                                                        \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_49(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50) >

#define PARTITION_LIST_51(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51)                                                                   \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_50(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51) >

#define PARTITION_LIST_52(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52)                                                              \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_51(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52) >

#define PARTITION_LIST_53(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53)                                                         \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_52(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53) >

#define PARTITION_LIST_54(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54)                                                    \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_53(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54) >

#define PARTITION_LIST_55(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55)                                               \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_54(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55) >

#define PARTITION_LIST_56(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56)                                          \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_55(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56) >

#define PARTITION_LIST_57(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57)                                     \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_56(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57) >

#define PARTITION_LIST_58(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58)                                \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_57(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58)  \
        >

#define PARTITION_LIST_59(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59)                           \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_58(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59) >

#define PARTITION_LIST_60(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60)                      \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_59(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60) >

#define PARTITION_LIST_61(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61)                 \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_60(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61) >

#define PARTITION_LIST_62(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62)            \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_61(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62) >

#define PARTITION_LIST_63(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63)       \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_62(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63) >

#define PARTITION_LIST_64(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64)  \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_63(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64) >

#define PARTITION_LIST_65(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65)                                                                                                           \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_64(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65) >

#define PARTITION_LIST_66(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66)                                                                                                      \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_65(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66) >

#define PARTITION_LIST_67(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67)                                                                                                 \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_66(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67) >

#define PARTITION_LIST_68(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68)                                                                                            \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_67(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68) >

#define PARTITION_LIST_69(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69)                                                                                       \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_68(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69) >

#define PARTITION_LIST_70(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70)                                                                                  \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_69(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70) >

#define PARTITION_LIST_71(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71)                                                                             \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_70(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71) >

#define PARTITION_LIST_72(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72)                                                                        \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_71(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72) >

#define PARTITION_LIST_73(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73)                                                                   \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_72(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73) >

#define PARTITION_LIST_74(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74)                                                              \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_73(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74) >

#define PARTITION_LIST_75(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75)                                                         \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_74(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75) >

#define PARTITION_LIST_76(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76)                                                    \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_75(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76) >

#define PARTITION_LIST_77(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77)                                               \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_76(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77) >

#define PARTITION_LIST_78(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78)                                          \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_77(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78) >

#define PARTITION_LIST_79(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79)                                     \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_78(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79) >

#define PARTITION_LIST_80(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80)                                \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_79(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80)  \
        >

#define PARTITION_LIST_81(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81)                           \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_80(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81) >

#define PARTITION_LIST_82(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82)                      \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_81(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82) >

#define PARTITION_LIST_83(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83)                 \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_82(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83) >

#define PARTITION_LIST_84(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84)            \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_83(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84) >

#define PARTITION_LIST_85(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85)       \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_84(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85) >

#define PARTITION_LIST_86(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86)  \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_85(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86) >

#define PARTITION_LIST_87(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87)                                                                                                           \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_86(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87) >

#define PARTITION_LIST_88(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88)                                                                                                      \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_87(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88) >

#define PARTITION_LIST_89(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89)                                                                                                 \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_88(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89) >

#define PARTITION_LIST_90(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90)                                                                                            \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_89(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90) >

#define PARTITION_LIST_91(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91)                                                                                       \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_90(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91) >

#define PARTITION_LIST_92(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92)                                                                                  \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_91(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92) >

#define PARTITION_LIST_93(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93)                                                                             \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_92(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93) >

#define PARTITION_LIST_94(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93, T94)                                                                        \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_93(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94) >

#define PARTITION_LIST_95(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93, T94, T95)                                                                   \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_94(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95) >

#define PARTITION_LIST_96(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93, T94, T95, T96)                                                              \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_95(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96) >

#define PARTITION_LIST_97(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97)                                                         \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_96(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97) >

#define PARTITION_LIST_98(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98)                                                    \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_97(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98) >

#define PARTITION_LIST_99(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,   \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99)                                               \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_98(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99) >

#define PARTITION_LIST_100(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,  \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100)                                         \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_99(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,  \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100) >

#define PARTITION_LIST_101(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,  \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101)                                   \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_100(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101) >

#define PARTITION_LIST_102(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,  \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101, T102)                             \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_101(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101,     \
        T102) >

#define PARTITION_LIST_103(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,  \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101, T102, T103)                       \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_102(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101,     \
        T102, T103) >

#define PARTITION_LIST_104(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,  \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101, T102, T103, T104)                 \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_103(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101,     \
        T102, T103, T104) >

#define PARTITION_LIST_105(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,  \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101, T102, T103, T104, T105)           \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_104(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101,     \
        T102, T103, T104, T105) >

#define PARTITION_LIST_106(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,  \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101, T102, T103, T104, T105, T106)     \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_105(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101,     \
        T102, T103, T104, T105, T106) >

#define PARTITION_LIST_107(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,  \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101, T102, T103, T104, T105, T106,     \
        T107)                                                                                                          \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_106(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101,     \
        T102, T103, T104, T105, T106, T107) >

#define PARTITION_LIST_108(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,  \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101, T102, T103, T104, T105, T106,     \
        T107, T108)                                                                                                    \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_107(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101,     \
        T102, T103, T104, T105, T106, T107, T108) >

#define PARTITION_LIST_109(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,  \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101, T102, T103, T104, T105, T106,     \
        T107, T108, T109)                                                                                              \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_108(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101,     \
        T102, T103, T104, T105, T106, T107, T108, T109) >

#define PARTITION_LIST_110(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,  \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101, T102, T103, T104, T105, T106,     \
        T107, T108, T109, T110)                                                                                        \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_109(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101,     \
        T102, T103, T104, T105, T106, T107, T108, T109, T110) >

#define PARTITION_LIST_111(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,  \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101, T102, T103, T104, T105, T106,     \
        T107, T108, T109, T110, T111)                                                                                  \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_110(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101,     \
        T102, T103, T104, T105, T106, T107, T108, T109, T110, T111) >

#define PARTITION_LIST_112(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,  \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101, T102, T103, T104, T105, T106,     \
        T107, T108, T109, T110, T111, T112)                                                                            \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_111(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101,     \
        T102, T103, T104, T105, T106, T107, T108, T109, T110, T111, T112) >

#define PARTITION_LIST_113(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,  \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101, T102, T103, T104, T105, T106,     \
        T107, T108, T109, T110, T111, T112, T113)                                                                      \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_112(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101,     \
        T102, T103, T104, T105, T106, T107, T108, T109, T110, T111, T112, T113) >

#define PARTITION_LIST_114(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,  \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101, T102, T103, T104, T105, T106,     \
        T107, T108, T109, T110, T111, T112, T113, T114)                                                                \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_113(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101,     \
        T102, T103, T104, T105, T106, T107, T108, T109, T110, T111, T112, T113, T114) >

#define PARTITION_LIST_115(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,  \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101, T102, T103, T104, T105, T106,     \
        T107, T108, T109, T110, T111, T112, T113, T114, T115)                                                          \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_114(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101,     \
        T102, T103, T104, T105, T106, T107, T108, T109, T110, T111, T112, T113, T114, T115) >

#define PARTITION_LIST_116(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,  \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101, T102, T103, T104, T105, T106,     \
        T107, T108, T109, T110, T111, T112, T113, T114, T115, T116)                                                    \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_115(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101,     \
        T102, T103, T104, T105, T106, T107, T108, T109, T110, T111, T112, T113, T114, T115, T116) >

#define PARTITION_LIST_117(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,  \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101, T102, T103, T104, T105, T106,     \
        T107, T108, T109, T110, T111, T112, T113, T114, T115, T116, T117)                                              \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_116(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101,     \
        T102, T103, T104, T105, T106, T107, T108, T109, T110, T111, T112, T113, T114, T115, T116, T117) >

#define PARTITION_LIST_118(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,  \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101, T102, T103, T104, T105, T106,     \
        T107, T108, T109, T110, T111, T112, T113, T114, T115, T116, T117, T118)                                        \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_117(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101,     \
        T102, T103, T104, T105, T106, T107, T108, T109, T110, T111, T112, T113, T114, T115, T116, T117, T118) >

#define PARTITION_LIST_119(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,  \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101, T102, T103, T104, T105, T106,     \
        T107, T108, T109, T110, T111, T112, T113, T114, T115, T116, T117, T118, T119)                                  \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_118(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101,     \
        T102, T103, T104, T105, T106, T107, T108, T109, T110, T111, T112, T113, T114, T115, T116, T117, T118, T119) >

#define PARTITION_LIST_120(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,  \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101, T102, T103, T104, T105, T106,     \
        T107, T108, T109, T110, T111, T112, T113, T114, T115, T116, T117, T118, T119, T120)                            \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_119(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101,     \
        T102, T103, T104, T105, T106, T107, T108, T109, T110, T111, T112, T113, T114, T115, T116, T117, T118, T119,    \
        T120) >

#define PARTITION_LIST_121(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,  \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101, T102, T103, T104, T105, T106,     \
        T107, T108, T109, T110, T111, T112, T113, T114, T115, T116, T117, T118, T119, T120, T121)                      \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_120(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101,     \
        T102, T103, T104, T105, T106, T107, T108, T109, T110, T111, T112, T113, T114, T115, T116, T117, T118, T119,    \
        T120, T121) >

#define PARTITION_LIST_122(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,  \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101, T102, T103, T104, T105, T106,     \
        T107, T108, T109, T110, T111, T112, T113, T114, T115, T116, T117, T118, T119, T120, T121, T122)                \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_121(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101,     \
        T102, T103, T104, T105, T106, T107, T108, T109, T110, T111, T112, T113, T114, T115, T116, T117, T118, T119,    \
        T120, T121, T122) >

#define PARTITION_LIST_123(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,  \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101, T102, T103, T104, T105, T106,     \
        T107, T108, T109, T110, T111, T112, T113, T114, T115, T116, T117, T118, T119, T120, T121, T122, T123)          \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_122(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101,     \
        T102, T103, T104, T105, T106, T107, T108, T109, T110, T111, T112, T113, T114, T115, T116, T117, T118, T119,    \
        T120, T121, T122, T123) >

#define PARTITION_LIST_124(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,  \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101, T102, T103, T104, T105, T106,     \
        T107, T108, T109, T110, T111, T112, T113, T114, T115, T116, T117, T118, T119, T120, T121, T122, T123, T124)    \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_123(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101,     \
        T102, T103, T104, T105, T106, T107, T108, T109, T110, T111, T112, T113, T114, T115, T116, T117, T118, T119,    \
        T120, T121, T122, T123, T124) >

#define PARTITION_LIST_125(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,  \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101, T102, T103, T104, T105, T106,     \
        T107, T108, T109, T110, T111, T112, T113, T114, T115, T116, T117, T118, T119, T120, T121, T122, T123, T124,    \
        T125)                                                                                                          \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_124(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101,     \
        T102, T103, T104, T105, T106, T107, T108, T109, T110, T111, T112, T113, T114, T115, T116, T117, T118, T119,    \
        T120, T121, T122, T123, T124, T125) >

#define PARTITION_LIST_126(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,  \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42,  \
        T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58, T59, T60, T61, T62, T63, T64,  \
        T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80, T81, T82, T83, T84, T85, T86,  \
        T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101, T102, T103, T104, T105, T106,     \
        T107, T108, T109, T110, T111, T112, T113, T114, T115, T116, T117, T118, T119, T120, T121, T122, T123, T124,    \
        T125, T126)                                                                                                    \
    ::BugEngine::World::PartitionList< T1, PARTITION_LIST_125(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, \
        T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34, T35, T36,  \
        T37, T38, T39, T40, T41, T42, T43, T44, T45, T46, T47, T48, T49, T50, T51, T52, T53, T54, T55, T56, T57, T58,  \
        T59, T60, T61, T62, T63, T64, T65, T66, T67, T68, T69, T70, T71, T72, T73, T74, T75, T76, T77, T78, T79, T80,  \
        T81, T82, T83, T84, T85, T86, T87, T88, T89, T90, T91, T92, T93, T94, T95, T96, T97, T98, T99, T100, T101,     \
        T102, T103, T104, T105, T106, T107, T108, T109, T110, T111, T112, T113, T114, T115, T116, T117, T118, T119,    \
        T120, T121, T122, T123, T124, T125, T126) >

/*****************************************************************************/
#endif
