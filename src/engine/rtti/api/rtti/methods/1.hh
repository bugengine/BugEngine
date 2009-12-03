/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_METHODS_1_HH_
#define BE_RTTI_METHODS_1_HH_
/*****************************************************************************/
#include    <typeinfo>
#include    <minitl/type/typemanipulation.hh>

namespace BugEngine { namespace RTTI { namespace _
{



template< typename OWNER, typename R, typename T1, R(OWNER::*Func)(T1) >
class MethodR1 : public Method
{
private:
    class MetaClass : public  Method::MetaClass
    {
    public:
        MetaClass() : Method::MetaClass(typeid(Func).name(), Method::static_metaclass(), false) {}

        virtual Value call(Value* values, size_t numvalues) const override
        {
            be_assert(numvalues == 2, "expected %d parameters; got %d" | 2 | numvalues);
            weak<OWNER> o = Marshaller< weak<OWNER> >().castto(values[0]);
            const typename minitl::remove_reference<T1>::type& v1 = Marshaller<T1>().castto(values[1]);
            const typename minitl::remove_reference<R>::type& v = (o.operator->()->*Func)(v1);
            return Value(Marshaller<R>().castfrom(v));
        }
    };
public:
    virtual ref<const RTTI::MetaClass> metaclass() const override { return static_metaclass(); }
    static  ref<const MetaClass> static_metaclass()               { static ref<MetaClass> s_metaclass = ref<MetaClass>::create(); return s_metaclass; }
};




template< typename OWNER, typename R, typename T1, R(OWNER::*Func)(T1) const >
class MethodCR1 : public Method
{
private:
    class MetaClass : public  Method::MetaClass
    {
    public:
        MetaClass() : Method::MetaClass(typeid(Func).name(), Method::static_metaclass(), false) {}

        virtual Value call(Value* values, size_t numvalues) const override
        {
            be_assert(numvalues == 2, "expected %d parameters; got %d" | 2 | numvalues);
            weak<OWNER> o = Marshaller< weak<OWNER> >().castto(values[0]);
            const typename minitl::remove_reference<T1>::type& v1 = Marshaller<T1>().castto(values[1]);
            const typename minitl::remove_reference<R>::type& v = (o.operator->()->*Func)(v1);
            return Value(Marshaller<R>().castfrom(v));
        }
    };
public:
    virtual ref<const RTTI::MetaClass> metaclass() const override { return static_metaclass(); }
    static  ref<const MetaClass> static_metaclass()               { static ref<MetaClass> s_metaclass = ref<MetaClass>::create(); return s_metaclass; }
};



template< typename OWNER, typename T1, void(OWNER::*Func)(T1) >
class MethodR1<OWNER, void, T1, Func> : public Method
{
private:
    class MetaClass : public  Method::MetaClass
    {
    public:
        MetaClass() : Method::MetaClass(typeid(Func).name(), Method::static_metaclass(), false) {}

        virtual Value call(Value* values, size_t numvalues) const override
        {
            be_assert(numvalues == 2, "expected %d parameters; got %d" | 2 | numvalues);
            weak<OWNER> o = Marshaller< weak<OWNER> >().castto(values[0]);
            const typename minitl::remove_reference<T1>::type& v1 = Marshaller<T1>().castto(values[1]);
            (o.operator->()->*Func)(v1);
            return Value();
        }
    };
public:
    virtual ref<const RTTI::MetaClass> metaclass() const override { return static_metaclass(); }
    static  ref<const MetaClass> static_metaclass()               { static ref<MetaClass> s_metaclass = ref<MetaClass>::create(); return s_metaclass; }
};




template< typename OWNER, typename T1, void(OWNER::*Func)(T1) const >
class MethodCR1<OWNER, void, T1, Func> : public Method
{
private:
    class MetaClass : public  Method::MetaClass
    {
    public:
        MetaClass() : Method::MetaClass(typeid(Func).name(), Method::static_metaclass(), false) {}

        virtual Value call(Value* values, size_t numvalues) const override
        {
            be_assert(numvalues == 2, "expected %d parameters; got %d" | 2 | numvalues);
            weak<OWNER> o = Marshaller< weak<OWNER> >().castto(values[0]);
            const typename minitl::remove_reference<T1>::type& v1 = Marshaller<T1>().castto(values[1]);
            (o.operator->()->*Func)(v1);
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
