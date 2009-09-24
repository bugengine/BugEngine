/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_METHODS_0_HH_
#define BE_RTTI_METHODS_0_HH_
/*****************************************************************************/
#include    <typeinfo>

namespace BugEngine { namespace RTTI { namespace _
{

template< typename OWNER, typename R, R(OWNER::*Func)() >
class MethodR0 : public Method
{
private:
    class MetaClass : public  Method::MetaClass
    {
    public:
        MetaClass() : Method::MetaClass(typeid(Func).name(), Method::static_metaclass(), false) {}

        virtual Value call(Value* values, size_t numvalues) const override
        {
            be_assert(numvalues == 1, "expected %d parameters; got %d" | 1 | numvalues);
            OWNER* o = Marshaller<OWNER*>().castto(values[0]);
            const typename minitl::remove_reference<R>::type& v = (o->*Func)();
            return Value(Marshaller<R>().castfrom(v));
        }
    };
public:
    virtual const MetaClass* metaclass() const override { return static_metaclass(); }
    static  const MetaClass* static_metaclass()         { static MetaClass s_metaclass; return &s_metaclass; }
};

template< typename OWNER, typename R, R(OWNER::*Func)() const >
class MethodCR0 : public Method
{
private:
    class MetaClass : public  Method::MetaClass
    {
    public:
        MetaClass() : Method::MetaClass(typeid(Func).name(), Method::static_metaclass(), false) {}

        virtual Value call(Value* values, size_t numvalues) const override
        {
            be_assert(numvalues == 1, "expected %d parameters; got %d" | 1 | numvalues);
            OWNER* o = Marshaller<OWNER*>().castto(values[0]);
            const typename minitl::remove_reference<R>::type& v = (o->*Func)();
            return Value(Marshaller<R>().castfrom(v));
        }
    };
public:
    virtual const MetaClass* metaclass() const override { return static_metaclass(); }
    static  const MetaClass* static_metaclass()         { static MetaClass s_metaclass; return &s_metaclass; }
};


template< typename OWNER, void(OWNER::*Func)() >
class MethodR0< OWNER, void, Func > : public Method
{
private:
    class MetaClass : public Method::MetaClass
    {
    public:
        MetaClass() : Method::MetaClass(typeid(Func).name(), Method::static_metaclass(), false) {}

        virtual Value call(Value* values, size_t numvalues) const override
        {
            be_assert(numvalues == 1, "expected %d parameters; got %d" | 1 | numvalues);
            OWNER* o = Marshaller<OWNER*>().castto(values[0]);
            (o->*Func)();
            return Value();
        }
    };
public:
    virtual const MetaClass* metaclass() const override { return static_metaclass(); }
    static  const MetaClass* static_metaclass()         { static MetaClass s_metaclass; return &s_metaclass; }
};

template< typename OWNER, void(OWNER::*Func)() const >
class MethodCR0< OWNER, void, Func > : public Method
{
private:
    class MetaClass : public Method::MetaClass
    {
    public:
        MetaClass() : Method::MetaClass(typeid(Func).name(), Method::static_metaclass(), false) {}

        virtual Value call(Value* values, size_t numvalues) const override
        {
            be_assert(numvalues == 1, "expected %d parameters; got %d" | 1 | numvalues);
            OWNER* o = Marshaller<OWNER*>().castto(values[0]);
            (o->*Func)();
            return Value();
        }
    };
public:
    virtual const MetaClass* metaclass() const override { return static_metaclass(); }
    static  const MetaClass* static_metaclass()         { static MetaClass s_metaclass; return &s_metaclass; }
};

}}}

/*****************************************************************************/
#endif
