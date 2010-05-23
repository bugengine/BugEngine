/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include   <rtti/stdafx.h>
#include   <rtti/typeinfo.hh>
#include   <rtti/engine/struct.hh>
#include   <rtti/engine/enum.hh>

namespace Example
{

class Test
{
public:
    enum Type
    {
        Type_Fun
    };
    void doStuff() const
    {
        OutputDebugString(minitl::format<>("%d\n") | i);
    }

    Test(int i) : i(i) { }
    ~Test() { }

    int i;
    Type type;
};


}

namespace BugEngine
{

be_enum(Example::Test::Type, "Example.Type")
    be_value_name(Example::Test::Type_Fun, Type_Fun)
be_end

//be_class(Example::Test, void, "Example.Test")
//    [Documentation::Brief("Example class")]
//    [Documentation::Doc("Example class")]
//
//    be_field(i)
//        [Documentation::Brief("Example property")]
//    be_field(type)
//        [Documentation::Brief("Example type")]
//be_end

template<>
struct be_typeid<Example::Test>
{
    static ref<const ClassInfo>   s_class;
    static ref<const ClassInfo> classInfo()
    {
        static ref<const ClassInfo> klass = ref< RTTI::ClassInfo<Example::Test> >::create("Example.Test");
        return klass;
    }
    static BE_EXPORT TypeInfo type()
    {
        return TypeInfo(classInfo());
    }
};
ref<const ClassInfo> be_typeid<Example::Test>::s_class = be_typeid<Example::Test>::classInfo();

static struct Startup
{
    Startup()
    {
        Example::Test t(2);
        Example::Test& t2 = t;
        const Example::Test& t3 = t;
        float f[4] = { 0.0f };
        Value v = Value(t);
        Value v2 = Value(t2);
        Value v3 = Value(t3);
        Value v4 = Value(f);
    }
} s_startup;
}
