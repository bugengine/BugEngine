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

#ifndef BE_RTTI_TEST_HH_
#define BE_RTTI_TEST_HH_
/*****************************************************************************/
#include    <rtti/helper.hh>

namespace TestNS
{

class RTTIEXPORT Test : public BugEngine::Object
{
private:
    int m_value;
    refptr<Test> m_other;
public:
    Test();
    ~Test();

    u8& prop() const;
    void setProp(u8 value);
    void setValue(int v);
    void setValue2(const int& v);

    void test(const std::string& str) const;

    be_metaclass(RTTIEXPORT,Test,BugEngine::Object)
    be_properties
        be_property(Value)
            [be_read(m_value)]
            [be_write(m_value)];

        be_property(Value2)
            [be_read(m_value)]
            [be_set(setValue2)];

        be_property(prop)
            [be_get(prop)]
            [be_set(setProp)];

        be_method(test);
    be_end
};

class RTTIEXPORT Test2 : public Test
{
public:
    Test2();
    ~Test2();

    be_metaclass(RTTIEXPORT,Test2,Test)
    be_properties
    be_end
};
}

/*****************************************************************************/
#endif
