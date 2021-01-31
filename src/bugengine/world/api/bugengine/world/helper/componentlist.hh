/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_WORLD_HELPER_COMPONENTLIST_HH_
#define BE_WORLD_HELPER_COMPONENTLIST_HH_
/**************************************************************************************************/
#include <bugengine/world/stdafx.h>
#include <bugengine/rtti/engine/propertyinfo.script.hh>
#include <bugengine/rtti/value.hh>
#include <bugengine/scheduler/kernel/parameters/segments.hh>
#include <bugengine/scheduler/kernel/product.hh>
#include <bugengine/world/helper/partitionlist.hh>

namespace BugEngine { namespace World {

enum StorageSize
{
    StorageSize_4k   = 0,
    StorageSize_16k  = 1,
    StorageSize_64k  = 2,
    StorageSize_256k = 3
};

template < typename T, StorageSize STORAGE, typename TAIL >
struct ComponentList : public TAIL
{
    enum
    {
        Index   = TAIL::Index + 1,
        Storage = (u32)STORAGE
    };
    typedef T                                                         Type;
    typedef TAIL                                                      Tail;
    ref< KernelScheduler::Product< KernelScheduler::Segments< T > > > product;
    ComponentList(weak< Task::ITask > task)
        : TAIL(task)
        , product(ref< KernelScheduler::Product< KernelScheduler::Segments< T > > >::create(
              Arena::game(), ref< KernelScheduler::Segments< T > >::create(Arena::game()), task))
    {
    }
    static void
    addComponent(minitl::array< minitl::tuple< raw< const RTTI::Class >, u32 > >& componentList,
                 u32                                                              count = 0)
    {
        componentList[count].first  = be_class< T >();
        componentList[count].second = STORAGE;
        TAIL::addComponent(componentList, count + 1);
    }

private:
    ComponentList(const ComponentList& other);
    ComponentList& operator=(const ComponentList& other);
};

template < typename T, StorageSize STORAGE >
struct ComponentList< T, STORAGE, void >
{
    enum
    {
        Index   = 0,
        Storage = STORAGE
    };
    typedef T                                                         Type;
    typedef void                                                      Tail;
    ref< KernelScheduler::Product< KernelScheduler::Segments< T > > > product;
    ComponentList(weak< Task::ITask > task)
        : product(ref< KernelScheduler::Product< KernelScheduler::Segments< T > > >::create(
            Arena::game(), ref< KernelScheduler::Segments< T > >::create(Arena::game()), task))
    {
    }
    static void
    addComponent(minitl::array< minitl::tuple< raw< const RTTI::Class >, u32 > >& componentList,
                 u32                                                              count = 0)
    {
        componentList[count].first  = be_class< T >();
        componentList[count].second = STORAGE;
    }
};

namespace Helper {

template < typename T, typename T2, StorageSize STORAGE, typename TAIL >
struct ProductGetter
{
    static weak< const KernelScheduler::Product< KernelScheduler::Segments< T > > >
    getProduct(const ComponentList< T2, STORAGE, TAIL >& list)
    {
        return ProductGetter< T, typename TAIL::Type, (StorageSize)TAIL::Storage,
                              typename TAIL::Tail >::getProduct(list);
    }
};

template < typename T, StorageSize STORAGE, typename TAIL >
struct ProductGetter< T, T, STORAGE, TAIL >
{
    static weak< const KernelScheduler::Product< KernelScheduler::Segments< T > > >
    getProduct(const ComponentList< T, STORAGE, TAIL >& list)
    {
        return list.product;
    }
};

template < typename LIST, u32 INDEX, typename T, StorageSize STORAGE, typename TAIL >
struct Property
{
    static inline void fillProperty(RTTI::Property properties[])
    {
        typedef Property< LIST, INDEX + 1, typename TAIL::Type, (StorageSize)TAIL::Storage,
                          typename TAIL::Tail >
                       PropertyParent;
        RTTI::Property property = {
            {0},
            be_class< T >()->name,
            be_type< LIST >(),
            be_type< weak< const KernelScheduler::Product< KernelScheduler::Segments< T > > > >(),
            &LIST::template getProduct< T >};
        new(&properties[INDEX]) RTTI::Property(property);
        PropertyParent::fillProperty(properties);
    }
};

template < typename LIST, u32 INDEX, typename T, StorageSize STORAGE >
struct Property< LIST, INDEX, T, STORAGE, void >
{
    static inline void fillProperty(RTTI::Property properties[])
    {
        RTTI::Property property = {
            {0},
            be_class< T >()->name,
            be_type< LIST >(),
            be_type< weak< const KernelScheduler::Product< KernelScheduler::Segments< T > > > >(),
            &LIST::template getProduct< T >};
        new(&properties[INDEX]) RTTI::Property(property);
    }
};

}  // namespace Helper

template <
    typename T1 = void, StorageSize S1 = StorageSize_4k, typename T2 = void,
    StorageSize S2 = StorageSize_4k, typename T3 = void, StorageSize S3 = StorageSize_4k,
    typename T4 = void, StorageSize S4 = StorageSize_4k, typename T5 = void,
    StorageSize S5 = StorageSize_4k, typename T6 = void, StorageSize S6 = StorageSize_4k,
    typename T7 = void, StorageSize S7 = StorageSize_4k, typename T8 = void,
    StorageSize S8 = StorageSize_4k, typename T9 = void, StorageSize S9 = StorageSize_4k,
    typename T10 = void, StorageSize S10 = StorageSize_4k, typename T11 = void,
    StorageSize S11 = StorageSize_4k, typename T12 = void, StorageSize S12 = StorageSize_4k,
    typename T13 = void, StorageSize S13 = StorageSize_4k, typename T14 = void,
    StorageSize S14 = StorageSize_4k, typename T15 = void, StorageSize S15 = StorageSize_4k,
    typename T16 = void, StorageSize S16 = StorageSize_4k, typename T17 = void,
    StorageSize S17 = StorageSize_4k, typename T18 = void, StorageSize S18 = StorageSize_4k,
    typename T19 = void, StorageSize S19 = StorageSize_4k, typename T20 = void,
    StorageSize S20 = StorageSize_4k, typename T21 = void, StorageSize S21 = StorageSize_4k,
    typename T22 = void, StorageSize S22 = StorageSize_4k, typename T23 = void,
    StorageSize S23 = StorageSize_4k, typename T24 = void, StorageSize S24 = StorageSize_4k,
    typename T25 = void, StorageSize S25 = StorageSize_4k, typename T26 = void,
    StorageSize S26 = StorageSize_4k, typename T27 = void, StorageSize S27 = StorageSize_4k,
    typename T28 = void, StorageSize S28 = StorageSize_4k, typename T29 = void,
    StorageSize S29 = StorageSize_4k, typename T30 = void, StorageSize S30 = StorageSize_4k,
    typename T31 = void, StorageSize S31 = StorageSize_4k, typename T32 = void,
    StorageSize S32 = StorageSize_4k, typename T33 = void, StorageSize S33 = StorageSize_4k,
    typename T34 = void, StorageSize S34 = StorageSize_4k, typename T35 = void,
    StorageSize S35 = StorageSize_4k, typename T36 = void, StorageSize S36 = StorageSize_4k,
    typename T37 = void, StorageSize S37 = StorageSize_4k, typename T38 = void,
    StorageSize S38 = StorageSize_4k, typename T39 = void, StorageSize S39 = StorageSize_4k,
    typename T40 = void, StorageSize S40 = StorageSize_4k, typename T41 = void,
    StorageSize S41 = StorageSize_4k, typename T42 = void, StorageSize S42 = StorageSize_4k,
    typename T43 = void, StorageSize S43 = StorageSize_4k, typename T44 = void,
    StorageSize S44 = StorageSize_4k, typename T45 = void, StorageSize S45 = StorageSize_4k,
    typename T46 = void, StorageSize S46 = StorageSize_4k, typename T47 = void,
    StorageSize S47 = StorageSize_4k, typename T48 = void, StorageSize S48 = StorageSize_4k,
    typename T49 = void, StorageSize S49 = StorageSize_4k, typename T50 = void,
    StorageSize S50 = StorageSize_4k, typename T51 = void, StorageSize S51 = StorageSize_4k,
    typename T52 = void, StorageSize S52 = StorageSize_4k, typename T53 = void,
    StorageSize S53 = StorageSize_4k, typename T54 = void, StorageSize S54 = StorageSize_4k,
    typename T55 = void, StorageSize S55 = StorageSize_4k, typename T56 = void,
    StorageSize S56 = StorageSize_4k, typename T57 = void, StorageSize S57 = StorageSize_4k,
    typename T58 = void, StorageSize S58 = StorageSize_4k, typename T59 = void,
    StorageSize S59 = StorageSize_4k, typename T60 = void, StorageSize S60 = StorageSize_4k,
    typename T61 = void, StorageSize S61 = StorageSize_4k, typename T62 = void,
    StorageSize S62 = StorageSize_4k, typename T63 = void, StorageSize S63 = StorageSize_4k,
    typename T64 = void, StorageSize S64 = StorageSize_4k, typename T65 = void,
    StorageSize S65 = StorageSize_4k, typename T66 = void, StorageSize S66 = StorageSize_4k,
    typename T67 = void, StorageSize S67 = StorageSize_4k, typename T68 = void,
    StorageSize S68 = StorageSize_4k, typename T69 = void, StorageSize S69 = StorageSize_4k,
    typename T70 = void, StorageSize S70 = StorageSize_4k, typename T71 = void,
    StorageSize S71 = StorageSize_4k, typename T72 = void, StorageSize S72 = StorageSize_4k,
    typename T73 = void, StorageSize S73 = StorageSize_4k, typename T74 = void,
    StorageSize S74 = StorageSize_4k, typename T75 = void, StorageSize S75 = StorageSize_4k,
    typename T76 = void, StorageSize S76 = StorageSize_4k, typename T77 = void,
    StorageSize S77 = StorageSize_4k, typename T78 = void, StorageSize S78 = StorageSize_4k,
    typename T79 = void, StorageSize S79 = StorageSize_4k, typename T80 = void,
    StorageSize S80 = StorageSize_4k, typename T81 = void, StorageSize S81 = StorageSize_4k,
    typename T82 = void, StorageSize S82 = StorageSize_4k, typename T83 = void,
    StorageSize S83 = StorageSize_4k, typename T84 = void, StorageSize S84 = StorageSize_4k,
    typename T85 = void, StorageSize S85 = StorageSize_4k, typename T86 = void,
    StorageSize S86 = StorageSize_4k, typename T87 = void, StorageSize S87 = StorageSize_4k,
    typename T88 = void, StorageSize S88 = StorageSize_4k, typename T89 = void,
    StorageSize S89 = StorageSize_4k, typename T90 = void, StorageSize S90 = StorageSize_4k,
    typename T91 = void, StorageSize S91 = StorageSize_4k, typename T92 = void,
    StorageSize S92 = StorageSize_4k, typename T93 = void, StorageSize S93 = StorageSize_4k,
    typename T94 = void, StorageSize S94 = StorageSize_4k, typename T95 = void,
    StorageSize S95 = StorageSize_4k, typename T96 = void, StorageSize S96 = StorageSize_4k,
    typename T97 = void, StorageSize S97 = StorageSize_4k, typename T98 = void,
    StorageSize S98 = StorageSize_4k, typename T99 = void, StorageSize S99 = StorageSize_4k,
    typename T100 = void, StorageSize S100 = StorageSize_4k, typename T101 = void,
    StorageSize S101 = StorageSize_4k, typename T102 = void, StorageSize S102 = StorageSize_4k,
    typename T103 = void, StorageSize S103 = StorageSize_4k, typename T104 = void,
    StorageSize S104 = StorageSize_4k, typename T105 = void, StorageSize S105 = StorageSize_4k,
    typename T106 = void, StorageSize S106 = StorageSize_4k, typename T107 = void,
    StorageSize S107 = StorageSize_4k, typename T108 = void, StorageSize S108 = StorageSize_4k,
    typename T109 = void, StorageSize S109 = StorageSize_4k, typename T110 = void,
    StorageSize S110 = StorageSize_4k, typename T111 = void, StorageSize S111 = StorageSize_4k,
    typename T112 = void, StorageSize S112 = StorageSize_4k, typename T113 = void,
    StorageSize S113 = StorageSize_4k, typename T114 = void, StorageSize S114 = StorageSize_4k,
    typename T115 = void, StorageSize S115 = StorageSize_4k, typename T116 = void,
    StorageSize S116 = StorageSize_4k, typename T117 = void, StorageSize S117 = StorageSize_4k,
    typename T118 = void, StorageSize S118 = StorageSize_4k, typename T119 = void,
    StorageSize S119 = StorageSize_4k, typename T120 = void, StorageSize S120 = StorageSize_4k,
    typename T121 = void, StorageSize S121 = StorageSize_4k, typename T122 = void,
    StorageSize S122 = StorageSize_4k, typename T123 = void, StorageSize S123 = StorageSize_4k,
    typename T124 = void, StorageSize S124 = StorageSize_4k, typename T125 = void,
    StorageSize S125 = StorageSize_4k, typename T126 = void, StorageSize S126 = StorageSize_4k,
    typename T127 = void, StorageSize S127 = StorageSize_4k, typename T128 = void,
    StorageSize S128 = StorageSize_4k >
struct MakeComponentList
{
    typedef ComponentList<
        T1, S1,
        typename MakeComponentList<
            T2, S2, T3, S3, T4, S4, T5, S5, T6, S6, T7, S7, T8, S8, T9, S9, T10, S10, T11, S11, T12,
            S12, T13, S13, T14, S14, T15, S15, T16, S16, T17, S17, T18, S18, T19, S19, T20, S20,
            T21, S21, T22, S22, T23, S23, T24, S24, T25, S25, T26, S26, T27, S27, T28, S28, T29,
            S29, T30, S30, T31, S31, T32, S32, T33, S33, T34, S34, T35, S35, T36, S36, T37, S37,
            T38, S38, T39, S39, T40, S40, T41, S41, T42, S42, T43, S43, T44, S44, T45, S45, T46,
            S46, T47, S47, T48, S48, T49, S49, T50, S50, T51, S51, T52, S52, T53, S53, T54, S54,
            T55, S55, T56, S56, T57, S57, T58, S58, T59, S59, T60, S60, T61, S61, T62, S62, T63,
            S63, T64, S64, T65, S65, T66, S66, T67, S67, T68, S68, T69, S69, T70, S70, T71, S71,
            T72, S72, T73, S73, T74, S74, T75, S75, T76, S76, T77, S77, T78, S78, T79, S79, T80,
            S80, T81, S81, T82, S82, T83, S83, T84, S84, T85, S85, T86, S86, T87, S87, T88, S88,
            T89, S89, T90, S90, T91, S91, T92, S92, T93, S93, T94, S94, T95, S95, T96, S96, T97,
            S97, T98, S98, T99, S99, T100, S100, T101, S101, T102, S102, T103, S103, T104, S104,
            T105, S105, T106, S106, T107, S107, T108, S108, T109, S109, T110, S110, T111, S111,
            T112, S112, T113, S113, T114, S114, T115, S115, T116, S116, T117, S117, T118, S118,
            T119, S119, T120, S120, T121, S121, T122, S122, T123, S123, T124, S124, T125, S125,
            T126, S126, T127, S127, T128, S128 >::Result >
        Result;
};

template <>
struct MakeComponentList<>
{
    typedef void Result;
};

}}  // namespace BugEngine::World

/**************************************************************************************************/
#endif
