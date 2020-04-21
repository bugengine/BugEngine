/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_WORLD_HELPER_PARTITION_HH_
#define BE_WORLD_HELPER_PARTITION_HH_
/**************************************************************************************************/
#include    <world/stdafx.h>
#include    <rtti/engine/propertyinfo.script.hh>
#include    <rtti/value.hh>
#include    <scheduler/kernel/product.hh>
#include    <scheduler/kernel/parameters/segments.hh>


namespace BugEngine { namespace World
{

template< typename T, typename TAIL >
struct Partition;

namespace Helper
{

template< typename T, typename T2, typename TAIL >
struct PartitionProductGetter
{
    static weak<const KernelScheduler::Product< KernelScheduler::Segments<T> > > getProduct(const Partition<T2, TAIL>& partition)
    {
        return PartitionProductGetter<T, typename TAIL::Type, typename TAIL::Tail>::getProduct(partition);
    }
};

template< typename T, typename TAIL >
struct PartitionProductGetter<T, T, TAIL>
{
    static weak<const KernelScheduler::Product< KernelScheduler::Segments<T> > > getProduct(const Partition<T, TAIL>& partition)
    {
        return partition.product;
    }
};

}

template< typename T, typename TAIL >
struct Partition : public TAIL
{
    enum { Index = TAIL::Index+1 };
    typedef T Type;
    typedef TAIL Tail;
    ref< KernelScheduler::Product< KernelScheduler::Segments<T> > > product;
    Partition(weak<Task::ITask> task)
        :   TAIL(task)
        ,   product(ref<KernelScheduler::Product< KernelScheduler::Segments<T> > >::create(Arena::game(), ref< KernelScheduler::Segments<T> >::create(Arena::game()), task))
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
    static void buildList(minitl::array< raw<const RTTI::Class> >::iterator index)
    {
        *index = be_class<T>();
        Tail::buildList(index + 1);
    }
private:
    Partition(const Partition& other);
    Partition& operator=(const Partition& other);
};


template< typename T, typename TAIL >
const istring Partition<T, TAIL>::name()
{
    return istring(minitl::format<4096u>("%s+%s")
                   | be_class<T>()->name
                   | Partition<typename TAIL::Type, typename TAIL::Tail>::name());
}

template< typename T >
struct Partition<T, void>
{
public:
    enum { Index = 0 };
    typedef T Type;
    typedef void Tail;
    ref< KernelScheduler::Product< KernelScheduler::Segments<T> > > product;
    Partition(weak<Task::ITask> task)
        :   product(ref<KernelScheduler::Product< KernelScheduler::Segments<T> > >::create(Arena::game(), ref< KernelScheduler::Segments<T> >::create(Arena::game()), task))
    {
    }
    static const istring name();
    template< typename T2 >
    static RTTI::Value getProduct(void* from, bool isConst)
    {
        be_forceuse(isConst);
        const Partition* partition = static_cast<const Partition*>(from);
        return RTTI::Value(Helper::PartitionProductGetter<T2, T, Tail>::getProduct(*partition));
    }
    static void buildList(minitl::array< raw<const RTTI::Class> >::iterator index)
    {
        *index = be_class<T>();
    }

private:
    Partition(const Partition& other);
    Partition& operator=(const Partition& other);
};


template< typename T >
const istring Partition<T, void>::name()
{
    return be_class<T>()->name;
}


template< typename PARTITION, u32 INDEX, typename T, typename TAIL >
struct PartitionPropertyInfo
{
    static inline void fillProperty(RTTI::Property properties[])
    {
        typedef PartitionPropertyInfo<PARTITION,
                                      INDEX+1,
                                      typename TAIL::Type,
                                      typename TAIL::Tail> PropertyParent;
        RTTI::Property property = {
            {0},
            be_class<T>()->name,
            be_type<PARTITION>(),
            be_type< weak< const KernelScheduler::Product< KernelScheduler::Segments<T> > > >(),
            &PARTITION::template getProduct<T>
        };
        new (&properties[INDEX]) RTTI::Property(property);
        PropertyParent::fillProperty(properties);
    }
};

template< typename PARTITION, u32 INDEX,  typename T >
struct PartitionPropertyInfo<PARTITION, INDEX, T, void>
{
    static inline void fillProperty(RTTI::Property properties[])
    {
        RTTI::Property property = {
            {0},
            be_class<T>()->name,
            be_type<PARTITION>(),
            be_type< weak< const KernelScheduler::Product< KernelScheduler::Segments<T> > > >(),
            &PARTITION::template getProduct<T>
        };
        new (&properties[INDEX]) RTTI::Property(property);
    }
};

template< typename T >
struct Partition_BugHelper
{

    enum
    {
        PropertyCount = 1 + T::Index
    };
    static byte s_propertyBuffer[];
    static RTTI::Property* s_properties;

    static bool getPartitionProperties()
    {
        PartitionPropertyInfo<T,
                              0,
                              typename T::Type,
                              typename T::Tail>::fillProperty(s_properties);
        return true;
    }
};

template< typename T >
byte Partition_BugHelper<T>::s_propertyBuffer[Partition_BugHelper<T>::PropertyCount * sizeof(RTTI::Property)];

template< typename T >
RTTI::Property* Partition_BugHelper<T>::s_properties = reinterpret_cast<RTTI::Property*>(s_propertyBuffer);

template< typename T1 = void, typename T2 = void, typename T3 = void, typename T4 = void, typename T5 = void,
          typename T6 = void, typename T7 = void, typename T8 = void, typename T9 = void, typename T10 = void,
          typename T11 = void, typename T12 = void, typename T13 = void, typename T14 = void, typename T15 = void,
          typename T16 = void, typename T17 = void, typename T18 = void, typename T19 = void, typename T20 = void,
          typename T21 = void, typename T22 = void, typename T23 = void, typename T24 = void, typename T25 = void,
          typename T26 = void, typename T27 = void, typename T28 = void, typename T29 = void, typename T30 = void,
          typename T31 = void, typename T32 = void >
struct MakePartition
{
    typedef Partition< T1, typename MakePartition<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,
                                         T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27,
                                         T28, T29, T30, T31, T32 >::Result > Result;
};

template<>
struct MakePartition<>
{
    typedef void Result;
};

}}


namespace BugEngine { namespace RTTI
{

template< typename T, typename TAIL >
struct ClassID< World::Partition<T, TAIL> >
{
    BE_EXPORT static inline raw<const RTTI::Class> klass()
    {
        static const RTTI::Class s_class = {
            istring("Partition"),
            0,
            0,
            RTTI::ClassType_Object,
            {0},
            be_class<void>(),
            {0},
            {0},
            { World::Partition_BugHelper< World::Partition<T, TAIL> >::PropertyCount,
              World::Partition_BugHelper< World::Partition<T, TAIL> >::s_properties },
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
    BE_EXPORT static bool s_propertiesSet;
};


template< typename T, typename TAIL >
BE_EXPORT
bool ClassID< World::Partition<T, TAIL> >::s_propertiesSet = World::Partition_BugHelper< World::Partition<T, TAIL> >::getPartitionProperties();

}}

/**************************************************************************************************/
#endif
