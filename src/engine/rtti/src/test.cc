/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <rtti/stdafx.h>

#include    <rtti/test.hh>

namespace TestNS
{

be_metaclass_impl("",Test);
be_metaclass_impl("",Test2);
u8 s_value;

Test::Test()
:   m_value(14)
{
}

Test::~Test()
{
}

u8& Test::prop() const
{
    return s_value;
}

void Test::setProp(u8 value)
{
    s_value = value;
}

void Test::setValue(int v)
{
    m_value = v;
}

void Test::setValue2(const int& v)
{
    m_value = v;
}

void Test::test(const std::string& str) const
{
    printf("Hello, %s!!!\n", str.c_str());
}

Test2::Test2()
{
}

Test2::~Test2()
{
}

}
