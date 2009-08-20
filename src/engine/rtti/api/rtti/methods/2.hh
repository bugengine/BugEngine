/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

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
    class RTTIEXPORT MetaClass : public  Method::MetaClass
    {
    public:
        MetaClass() : Method::MetaClass(typeid(Func).name(), Method::static_metaclass(), false) {}

        virtual Value call(Value* values, size_t numvalues) const override
        {
            Assert(numvalues == 2);
            OWNER* o = Marshaller<OWNER*>().castto(values[0]);
            const typename minitl::remove_reference<T1>::type& v1 = Marshaller<T1>().castto(values[1]);
            const typename minitl::remove_reference<T2>::type& v2 = Marshaller<T1>().castto(values[2]);
            const R& v = (o->*Func)(v1, v2);
            return Value(Marshaller<R>().castfrom(v));
        }
    };
public:
    virtual const MetaClass* metaclass() const override { return static_metaclass(); }
    static  const MetaClass* static_metaclass()         { static MetaClass s_metaclass; return &s_metaclass; }
};




template< typename OWNER, typename R, typename T1, typename T2, R(OWNER::*Func)(T1,T2) const >
class MethodCR2 : public Method
{
private:
    class RTTIEXPORT MetaClass : public  Method::MetaClass
    {
    public:
        MetaClass() : Method::MetaClass(typeid(Func).name(), Method::static_metaclass(), false) {}

        virtual Value call(Value* values, size_t numvalues) const override
        {
            Assert(numvalues == 2);
            OWNER* o = Marshaller<OWNER*>().castto(values[0]);
            const typename minitl::remove_reference<T1>::type& v1 = Marshaller<T1>().castto(values[1]);
            const typename minitl::remove_reference<T2>::type& v2 = Marshaller<T1>().castto(values[2]);
            const R& v = (o->*Func)(v1, v2);
            return Value(Marshaller<R>().castfrom(v));
        }
    };
public:
    virtual const MetaClass* metaclass() const override { return static_metaclass(); }
    static  const MetaClass* static_metaclass()         { static MetaClass s_metaclass; return &s_metaclass; }
};




template< typename OWNER, typename T1, typename T2, void(OWNER::*Func)(T1,T2) >
class MethodR2<OWNER, void, T1, T2, Func> : public Method
{
private:
    class RTTIEXPORT MetaClass : public  Method::MetaClass
    {
    public:
        MetaClass() : Method::MetaClass(typeid(Func).name(), Method::static_metaclass(), false) {}

        virtual Value call(Value* values, size_t numvalues) const override
        {
            Assert(numvalues == 2);
            OWNER* o = Marshaller<OWNER*>().castto(values[0]);
            const typename minitl::remove_reference<T1>::type& v1 = Marshaller<T1>().castto(values[1]);
            const typename minitl::remove_reference<T2>::type& v2 = Marshaller<T1>().castto(values[2]);
            (o->*Func)(v1, v2);
            return Value();
        }
    };
public:
    virtual const MetaClass* metaclass() const override { return static_metaclass(); }
    static  const MetaClass* static_metaclass()         { static MetaClass s_metaclass; return &s_metaclass; }
};




template< typename OWNER, typename T1, typename T2, void(OWNER::*Func)(T1,T2) const >
class MethodCR2<OWNER, void, T1, T2, Func> : public Method
{
private:
    class RTTIEXPORT MetaClass : public  Method::MetaClass
    {
    public:
        MetaClass() : Method::MetaClass(typeid(Func).name(), Method::static_metaclass(), false) {}

        virtual Value call(Value* values, size_t numvalues) const override
        {
            Assert(numvalues == 2);
            OWNER* o = Marshaller<OWNER*>().castto(values[0]);
            const typename minitl::remove_reference<T1>::type& v1 = Marshaller<T1>().castto(values[1]);
            const typename minitl::remove_reference<T2>::type& v2 = Marshaller<T1>().castto(values[2]);
            (o->*Func)(v1, v2);
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
