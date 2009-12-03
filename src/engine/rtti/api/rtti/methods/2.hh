/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_METHODS_2_HH_
#define BE_RTTI_METHODS_2_HH_
/*****************************************************************************/
#include    <typeinfo>
#include    <minitl/type/typemanipulation.hh>

namespace BugEngine { namespace RTTI { namespace _
{


template< typename OWNER, typename R, typename T1, typename T2, R(OWNER::*Func)(T1,T2) >
class MethodR2 : public Method
{
private:
    class MetaClass : public  Method::MetaClass
    {
    public:
        MetaClass() : Method::MetaClass(typeid(Func).name(), Method::static_metaclass(), false) {}

        virtual Value call(Value* values, size_t numvalues) const override
        {
            be_assert(numvalues == 3, "expected %d parameters; got %d" | 3 | numvalues);
            weak<OWNER> o = Marshaller< weak<OWNER> >().castto(values[0]);
            const typename minitl::remove_reference<T1>::type& v1 = Marshaller<T1>().castto(values[1]);
            const typename minitl::remove_reference<T2>::type& v2 = Marshaller<T2>().castto(values[2]);
            const R& v = (o.operator->()->*Func)(v1, v2);
            return Value(Marshaller<R>().castfrom(v));
        }
    };
public:
    virtual ref<const RTTI::MetaClass> metaclass() const override { return static_metaclass(); }
    static  ref<const MetaClass> static_metaclass()               { static ref<MetaClass> s_metaclass = ref<MetaClass>::create(); return s_metaclass; }
};




template< typename OWNER, typename R, typename T1, typename T2, R(OWNER::*Func)(T1,T2) const >
class MethodCR2 : public Method
{
private:
    class MetaClass : public  Method::MetaClass
    {
    public:
        MetaClass() : Method::MetaClass(typeid(Func).name(), Method::static_metaclass(), false) {}

        virtual Value call(Value* values, size_t numvalues) const override
        {
            be_assert(numvalues == 3, "expected %d parameters; got %d" | 3 | numvalues);
            weak<OWNER> o = Marshaller< weak<OWNER> >().castto(values[0]);
            const typename minitl::remove_reference<T1>::type& v1 = Marshaller<T1>().castto(values[1]);
            const typename minitl::remove_reference<T2>::type& v2 = Marshaller<T2>().castto(values[2]);
            const R& v = (o.operator->()->*Func)(v1, v2);
            return Value(Marshaller<R>().castfrom(v));
        }
    };
public:
    virtual ref<const RTTI::MetaClass> metaclass() const override { return static_metaclass(); }
    static  ref<const MetaClass> static_metaclass()               { static ref<MetaClass> s_metaclass = ref<MetaClass>::create(); return s_metaclass; }
};




template< typename OWNER, typename T1, typename T2, void(OWNER::*Func)(T1,T2) >
class MethodR2<OWNER, void, T1, T2, Func> : public Method
{
private:
    class MetaClass : public  Method::MetaClass
    {
    public:
        MetaClass() : Method::MetaClass(typeid(Func).name(), Method::static_metaclass(), false) {}

        virtual Value call(Value* values, size_t numvalues) const override
        {
            be_assert(numvalues == 3, "expected %d parameters; got %d" | 3 | numvalues);
            weak<OWNER> o = Marshaller< weak<OWNER> >().castto(values[0]);
            const typename minitl::remove_reference<T1>::type& v1 = Marshaller<T1>().castto(values[1]);
            const typename minitl::remove_reference<T2>::type& v2 = Marshaller<T2>().castto(values[2]);
            (o.operator->()->*Func)(v1, v2);
            return Value();
        }
    };
public:
    virtual ref<const RTTI::MetaClass> metaclass() const override { return static_metaclass(); }
    static  ref<const MetaClass> static_metaclass()               { static ref<MetaClass> s_metaclass = ref<MetaClass>::create(); return s_metaclass; }
};




template< typename OWNER, typename T1, typename T2, void(OWNER::*Func)(T1,T2) const >
class MethodCR2<OWNER, void, T1, T2, Func> : public Method
{
private:
    class MetaClass : public  Method::MetaClass
    {
    public:
        MetaClass() : Method::MetaClass(typeid(Func).name(), Method::static_metaclass(), false) {}

        virtual Value call(Value* values, size_t numvalues) const override
        {
            be_assert(numvalues == 3, "expected %d parameters; got %d" | 3 | numvalues);
            weak<OWNER> o = Marshaller< weak<OWNER> >().castto(values[0]);
            const typename minitl::remove_reference<T1>::type& v1 = Marshaller<T1>().castto(values[1]);
            const typename minitl::remove_reference<T2>::type& v2 = Marshaller<T2>().castto(values[2]);
            (o.operator->()->*Func)(v1, v2);
            return Value();
        }
    };
public:
    virtual ref<const RTTI::MetaClass> metaclass() const override { return static_metaclass(); }
    static  ref<const MetaClass> static_metaclass()               { static ref<MetaClass> s_metaclass = ref<MetaClass>::create(); return s_metaclass; }
};


}}}

/*****************************************************************************/
#endif
