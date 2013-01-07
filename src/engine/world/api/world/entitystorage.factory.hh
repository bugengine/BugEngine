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
};
template<   typename T1,
            typename T2 = void,
            typename T3 = void,
            typename T4 = void,
            typename T5 = void,
            typename T6 = void,
            typename T7 = void,
            typename T8 = void,
            typename T9 = void,
            typename T10 = void,
            typename T11 = void,
            typename T12 = void,
            typename T13 = void,
            typename T14 = void,
            typename T15 = void,
            typename T16 = void,
            typename T17 = void,
            typename T18 = void,
            typename T19 = void,
            typename T20 = void,
            typename T21 = void,
            typename T22 = void,
            typename T23 = void,
            typename T24 = void,
            typename T25 = void,
            typename T26 = void,
            typename T27 = void,
            typename T28 = void,
            typename T29 = void,
            typename T30 = void,
            typename T31 = void,
            typename T32 = void,
            typename T33 = void,
            typename T34 = void,
            typename T35 = void,
            typename T36 = void,
            typename T37 = void,
            typename T38 = void,
            typename T39 = void,
            typename T40 = void,
            typename T41 = void,
            typename T42 = void,
            typename T43 = void,
            typename T44 = void,
            typename T45 = void,
            typename T46 = void,
            typename T47 = void,
            typename T48 = void,
            typename T49 = void,
            typename T50 = void >
class EntityStorageFactory : public EntityStorage
{
protected:
    typedef EntityStorageFactory<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10,
                                 T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,
                                 T21, T22, T23, T24, T25, T26, T27, T28, T29, T30,
                                 T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,
                                 T41, T42, T43, T44, T45, T46, T47, T48, T49, T50> ThisType;
    EntityStorageFactory()
        :   EntityStorage()
        ,   m_out1(initialTask())
        ,   m_out2(initialTask())
        ,   m_out3(initialTask())
        ,   m_out4(initialTask())
        ,   m_out5(initialTask())
        ,   m_out6(initialTask())
        ,   m_out7(initialTask())
        ,   m_out8(initialTask())
        ,   m_out9(initialTask())
        ,   m_out10(initialTask())
        ,   m_out11(initialTask())
        ,   m_out12(initialTask())
        ,   m_out13(initialTask())
        ,   m_out14(initialTask())
        ,   m_out15(initialTask())
        ,   m_out16(initialTask())
        ,   m_out17(initialTask())
        ,   m_out18(initialTask())
        ,   m_out19(initialTask())
        ,   m_out20(initialTask())
        ,   m_out21(initialTask())
        ,   m_out22(initialTask())
        ,   m_out23(initialTask())
        ,   m_out24(initialTask())
        ,   m_out25(initialTask())
        ,   m_out26(initialTask())
        ,   m_out27(initialTask())
        ,   m_out28(initialTask())
        ,   m_out29(initialTask())
        ,   m_out30(initialTask())
        ,   m_out31(initialTask())
        ,   m_out32(initialTask())
        ,   m_out33(initialTask())
        ,   m_out34(initialTask())
        ,   m_out35(initialTask())
        ,   m_out36(initialTask())
        ,   m_out37(initialTask())
        ,   m_out38(initialTask())
        ,   m_out39(initialTask())
        ,   m_out40(initialTask())
        ,   m_out41(initialTask())
        ,   m_out42(initialTask())
        ,   m_out43(initialTask())
        ,   m_out44(initialTask())
        ,   m_out45(initialTask())
        ,   m_out46(initialTask())
        ,   m_out47(initialTask())
        ,   m_out48(initialTask())
        ,   m_out49(initialTask())
        ,   m_out50(initialTask())
    {
    }
    static RTTI::Value getProperty1(void* from, bool isConst)
    {
        be_forceuse(isConst);
        return RTTI::Value(RTTI::Value::ByRef(((const ThisType*)from)->m_out1.product));
    }
    static RTTI::Value getProperty2(void* from, bool isConst)
    {
        be_forceuse(isConst);
        return RTTI::Value(RTTI::Value::ByRef(((const ThisType*)from)->m_out2.product));
    }
    static RTTI::Value getEmptyProperty(void* from, bool isConst)
    {
        be_forceuse(from);
        be_forceuse(isConst);
        return RTTI::Value();
    }
    const OutputStream<T1> m_out1;
    const OutputStream<T2> m_out2;
    const OutputStream<T3> m_out3;
    const OutputStream<T4> m_out4;
    const OutputStream<T5> m_out5;
    const OutputStream<T6> m_out6;
    const OutputStream<T7> m_out7;
    const OutputStream<T8> m_out8;
    const OutputStream<T9> m_out9;
    const OutputStream<T10> m_out10;
    const OutputStream<T11> m_out11;
    const OutputStream<T12> m_out12;
    const OutputStream<T13> m_out13;
    const OutputStream<T14> m_out14;
    const OutputStream<T15> m_out15;
    const OutputStream<T16> m_out16;
    const OutputStream<T17> m_out17;
    const OutputStream<T18> m_out18;
    const OutputStream<T19> m_out19;
    const OutputStream<T20> m_out20;
    const OutputStream<T21> m_out21;
    const OutputStream<T22> m_out22;
    const OutputStream<T23> m_out23;
    const OutputStream<T24> m_out24;
    const OutputStream<T25> m_out25;
    const OutputStream<T26> m_out26;
    const OutputStream<T27> m_out27;
    const OutputStream<T28> m_out28;
    const OutputStream<T29> m_out29;
    const OutputStream<T30> m_out30;
    const OutputStream<T31> m_out31;
    const OutputStream<T32> m_out32;
    const OutputStream<T33> m_out33;
    const OutputStream<T34> m_out34;
    const OutputStream<T35> m_out35;
    const OutputStream<T36> m_out36;
    const OutputStream<T37> m_out37;
    const OutputStream<T38> m_out38;
    const OutputStream<T39> m_out39;
    const OutputStream<T40> m_out40;
    const OutputStream<T41> m_out41;
    const OutputStream<T42> m_out42;
    const OutputStream<T43> m_out43;
    const OutputStream<T44> m_out44;
    const OutputStream<T45> m_out45;
    const OutputStream<T46> m_out46;
    const OutputStream<T47> m_out47;
    const OutputStream<T48> m_out48;
    const OutputStream<T49> m_out49;
    const OutputStream<T50> m_out50;
public:
    static const RTTI::Property s_properties[];
};

template< typename T1, typename T2, typename T3, typename T4, typename T5,
          typename T6, typename T7, typename T8, typename T9, typename T10,
          typename T11, typename T12, typename T13, typename T14, typename T15,
          typename T16, typename T17, typename T18, typename T19, typename T20,
          typename T21, typename T22, typename T23, typename T24, typename T25,
          typename T26, typename T27, typename T28, typename T29, typename T30,
          typename T31, typename T32, typename T33, typename T34, typename T35,
          typename T36, typename T37, typename T38, typename T39, typename T40,
          typename T41, typename T42, typename T43, typename T44, typename T45,
          typename T46, typename T47, typename T48, typename T49, typename T50 >
const RTTI::Property EntityStorageFactory<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10,
                                          T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,
                                          T21, T22, T23, T24, T25, T26, T27, T28, T29, T30,
                                          T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,
                                          T41, T42, T43, T44, T45, T46, T47, T48, T49, T50>::s_properties[50] =
{
    { {0}, {!minitl::type_equals<T2, void>::Value ? &s_properties[1] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T1>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T1>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T3, void>::Value ? &s_properties[2] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T2>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T2>& >::type(), minitl::type_equals<T2, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty2 },
    { {0}, {!minitl::type_equals<T4, void>::Value ? &s_properties[3] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T3>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T3>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T5, void>::Value ? &s_properties[4] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T4>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T4>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T6, void>::Value ? &s_properties[5] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T5>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T5>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T7, void>::Value ? &s_properties[6] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T6>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T6>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T8, void>::Value ? &s_properties[7] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T7>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T7>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T9, void>::Value ? &s_properties[8] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T8>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T8>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T10, void>::Value ? &s_properties[9] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T9>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T9>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T11, void>::Value ? &s_properties[10] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T10>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T10>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T12, void>::Value ? &s_properties[11] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T11>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T11>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T13, void>::Value ? &s_properties[12] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T12>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T12>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T14, void>::Value ? &s_properties[13] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T13>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T13>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T15, void>::Value ? &s_properties[14] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T14>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T14>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T16, void>::Value ? &s_properties[15] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T15>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T15>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T17, void>::Value ? &s_properties[16] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T16>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T16>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T18, void>::Value ? &s_properties[17] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T17>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T17>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T19, void>::Value ? &s_properties[18] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T18>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T18>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T20, void>::Value ? &s_properties[19] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T19>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T19>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T21, void>::Value ? &s_properties[20] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T20>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T20>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T22, void>::Value ? &s_properties[21] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T21>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T21>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T23, void>::Value ? &s_properties[22] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T22>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T22>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T24, void>::Value ? &s_properties[23] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T23>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T23>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T25, void>::Value ? &s_properties[24] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T24>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T24>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T26, void>::Value ? &s_properties[25] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T25>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T25>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T27, void>::Value ? &s_properties[26] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T26>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T26>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T28, void>::Value ? &s_properties[27] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T27>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T27>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T29, void>::Value ? &s_properties[28] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T28>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T28>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T30, void>::Value ? &s_properties[29] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T29>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T29>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T31, void>::Value ? &s_properties[30] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T30>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T30>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T32, void>::Value ? &s_properties[31] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T31>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T31>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T33, void>::Value ? &s_properties[32] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T32>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T32>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T34, void>::Value ? &s_properties[33] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T33>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T33>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T35, void>::Value ? &s_properties[34] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T34>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T34>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T36, void>::Value ? &s_properties[35] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T35>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T35>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T37, void>::Value ? &s_properties[36] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T36>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T36>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T38, void>::Value ? &s_properties[37] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T37>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T37>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T39, void>::Value ? &s_properties[38] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T38>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T38>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T40, void>::Value ? &s_properties[39] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T39>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T39>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T41, void>::Value ? &s_properties[40] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T40>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T40>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T42, void>::Value ? &s_properties[41] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T41>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T41>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T43, void>::Value ? &s_properties[42] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T42>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T42>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T44, void>::Value ? &s_properties[43] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T43>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T43>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T45, void>::Value ? &s_properties[44] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T44>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T44>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T46, void>::Value ? &s_properties[45] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T45>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T45>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T47, void>::Value ? &s_properties[46] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T46>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T46>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T48, void>::Value ? &s_properties[47] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T47>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T47>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T49, void>::Value ? &s_properties[48] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T48>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T48>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {!minitl::type_equals<T50, void>::Value ? &s_properties[49] : be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T49>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T49>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
    { {0}, {be_typeid<EntityStorage>::klass()->properties.m_ptr}, be_typeid<T50>::klass()->name, be_typeid<ThisType>::type(), minitl::type_equals<T1, void>::Value ? be_typeid<void>::type() : be_typeid< const Kernel::Product<T50>& >::type(), minitl::type_equals<T1, void>::Value ? &ThisType::getEmptyProperty : &ThisType::getProperty1 },
};

}}

namespace BugEngine
{

template<   typename T1,
            typename T2,
            typename T3,
            typename T4,
            typename T5,
            typename T6,
            typename T7,
            typename T8,
            typename T9,
            typename T10,
            typename T11,
            typename T12,
            typename T13,
            typename T14,
            typename T15,
            typename T16,
            typename T17,
            typename T18,
            typename T19,
            typename T20,
            typename T21,
            typename T22,
            typename T23,
            typename T24,
            typename T25,
            typename T26,
            typename T27,
            typename T28,
            typename T29,
            typename T30,
            typename T31,
            typename T32,
            typename T33,
            typename T34,
            typename T35,
            typename T36,
            typename T37,
            typename T38,
            typename T39,
            typename T40,
            typename T41,
            typename T42,
            typename T43,
            typename T44,
            typename T45,
            typename T46,
            typename T47,
            typename T48,
            typename T49,
            typename T50 >
struct be_typeid< World::EntityStorageFactory<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10,
                                       T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,
                                       T21, T22, T23, T24, T25, T26, T27, T28, T29, T30,
                                       T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,
                                       T41, T42, T43, T44, T45, T46, T47, T48, T49, T50> >
{
    static inline RTTI::Type  type()  { return RTTI::Type::makeType(klass(), RTTI::Type::Value, RTTI::Type::Mutable, RTTI::Type::Mutable); }
    static inline raw<const RTTI::Class> klass()
    {
        static const RTTI::Class s_class =
        {
            istring("EntityStorageFactory"),
            {0},
            be_typeid<World::EntityStorage>::klass(),
            0,
            0,
            {0},
            {&World::EntityStorageFactory<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10,
                                       T11, T12, T13, T14, T15, T16, T17, T18, T19, T20,
                                       T21, T22, T23, T24, T25, T26, T27, T28, T29, T30,
                                       T31, T32, T33, T34, T35, T36, T37, T38, T39, T40,
                                       T41, T42, T43, T44, T45, T46, T47, T48, T49, T50>::s_properties[0]},
            be_typeid<World::EntityStorage>::klass()->methods,
            be_typeid<World::EntityStorage>::klass()->objects,
            {0},
            {0},
            0,
            0
        };
        raw<const RTTI::Class> result = { &s_class };
        return result;
    }
};

}

/*****************************************************************************/
#endif
