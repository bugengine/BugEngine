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

be_struct(Example::Test, void, "Example.Test")
    [Documentation::Brief("Example class")]
    [Documentation::Doc("Example class")]

    be_field(i)
        [Documentation::Brief("Example property")]
    be_field(type)
        [Documentation::Brief("Example type")]
be_end

static struct Startup
{
    Startup()
    {
        Value v = Value(Example::Test(2));
        v["i"] = Value(42);
        v.as<Example::Test>().doStuff();
    }
} s_startup;
}
