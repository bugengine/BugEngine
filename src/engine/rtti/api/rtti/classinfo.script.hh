/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_CLASSINFO_HH_
#define BE_RTTI_CLASSINFO_HH_
/*****************************************************************************/
#include    <rtti/namespace.script.hh>
#include    <rtti/engine/propertyinfo.script.hh>
#include    <rtti/engine/methodinfo.script.hh>
#include    <rtti/value.hh>


namespace BugEngine
{
struct TypeInfo;
}

namespace BugEngine { namespace RTTI
{

struct PropertyInfo;
struct MethodInfo;


be_meta(ClassInfo::MetaClassInfo)
class be_api(RTTI) ClassInfo : public Namespace
{
    friend struct BugEngine::TypeInfo;
    friend class Namespace;
    friend class BugEngine::Value;
published:
    class be_api(RTTI) MetaClassInfo;
    friend class MetaClassInfo;
published:
    const inamespace                                name;
    const ref<const ClassInfo>                      parent;
    const u32                                       size;
    const i32                                       offset;
    minitl::hashmap< istring, const PropertyInfo>   properties;
    minitl::hashmap< istring, const MethodInfo>     methods;
    minitl::vector<Value>                           tags;
    MethodInfo                                      constructor;
    MethodInfo                                      call;
public:
    void (*copyconstructor)(const void* src, void* dst);
    void (*destructor)(void* src);
published:
    ClassInfo(const inamespace& name, ref<const ClassInfo> parent);
    ClassInfo(const inamespace& name, ref<const ClassInfo> parent, u32 size, i32 offset);
    ~ClassInfo();

published:
    void addProperty(istring name, const PropertyInfo& prop);
    void removeProperty(istring name);
    void addMethod(istring name, const MethodInfo& method);
    void removeMethod(istring name);

    virtual Value get(Value& from, istring name) const;

    Value getTag(const TypeInfo& type) const;
    bool isA(weak<const ClassInfo> klass) const;

    Value operator()(Value* params, size_t nparams) const;
public:
    template< typename T >
    static Value constructPtr0()
    {
        return Value(ref<T>::create(scriptArena()));
    }
    template< typename T, typename T1 >
    static Value constructPtr1(T1 t1)
    {
        return Value(ref<T>::create(scriptArena(), t1));
    }
    template< typename T, typename T1, typename T2 >
    static Value constructPtr2(T1 t1, T2 t2)
    {
        return Value(ref<T>::create(scriptArena(), t1, t2));
    }
    template< typename T, typename T1, typename T2, typename T3 >
    static Value constructPtr3(T1 t1, T2 t2, T3 t3)
    {
        return Value(ref<T>::create(scriptArena(), t1, t2, t3));
    }
    template< typename T, typename T1, typename T2, typename T3, typename T4 >
    static Value constructPtr4(T1 t1, T2 t2, T3 t3, T4 t4)
    {
        return Value(ref<T>::create(scriptArena(), t1, t2, t3, t4));
    }
    template< typename T, typename T1, typename T2, typename T3, typename T4, typename T5 >
    static Value constructPtr5(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5)
    {
        return Value(ref<T>::create(scriptArena(), t1, t2, t3, t4, t5));
    }
    template< typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6 >
    static Value constructPtr6(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6)
    {
        return Value(ref<T>::create(scriptArena(), t1, t2, t3, t4, t5, t6));
    }

    template< typename T >
    static Value construct0()
    {
        Value v(be_typeid<T>::type());
        new(v.memory()) T();
        return v;
    }
    template< typename T, typename T1 >
    static Value construct1(T1 t1)
    {
        Value v(be_typeid<T>::type());
        new(v.memory()) T(t1);
        return v;
    }
    template< typename T, typename T1, typename T2 >
    static Value construct2(T1 t1, T2 t2)
    {
        Value v(be_typeid<T>::type());
        new(v.memory()) T(t1, t2);
        return v;
    }
    template< typename T, typename T1, typename T2, typename T3 >
    static Value construct3(T1 t1, T2 t2, T3 t3)
    {
        Value v(be_typeid<T>::type());
        new(v.memory()) T(t1, t2, t3);
        return v;
    }
    template< typename T, typename T1, typename T2, typename T3, typename T4 >
    static Value construct4(T1 t1, T2& t2, T3 t3, T4 t4)
    {
        Value v(be_typeid<T>::type());
        new(v.memory()) T(t1, t2, t3, t4);
        return v;
    }
    template< typename T, typename T1, typename T2, typename T3, typename T4, typename T5 >
    static Value construct5(T1 t1, T2 t2, T3& t3, T4 t4, T5 t5)
    {
        Value v(be_typeid<T>::type());
        new(v.memory()) T(t1, t2, t3, t4, t5);
        return v;
    }
    template< typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6 >
    static Value construct6(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6)
    {
        Value v(be_typeid<T>::type());
        new(v.memory()) T(t1, t2, t3, t4, t5, t6);
        return v;
    }

private: // friend Value
    void copy(const void* src, void* dst) const;
    void destroy(void* src) const;
};

}}

#include    <rtti/engine/metaclassinfo.script.hh>

/*****************************************************************************/
#endif

