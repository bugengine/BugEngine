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
    friend BugEngine::be_typeid<Test>;
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
private:
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
//    [Documentation::Detail("Example class")]
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
        typedef const float float_array[4];
        float_array f = { 0.0f };
        const float_array * const f2 = &f;
        Value v = Value(t);
        Value v2 = Value(t2);
        Value v3 = Value(t3);
        Value v4 = Value(f2);
        OutputDebugString((minitl::format<>("%s %d\n")|be_typeid<const float_array * const* const>::type().name().c_str()|sizeof(v4)).c_str());
        OutputDebugString((minitl::format<>("%s %d\n")|v4.type().name().c_str()|sizeof(v4)).c_str());
    }
} s_startup;
}
