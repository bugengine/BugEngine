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

    u8 prop() const;
    void setProp(u8 value);

    METACLASS(RTTIEXPORT,Test,BugEngine::Object)
    PROPERTIES
        /*PROPERTY(Value)
            [DESCRIPTION=""]
            [READ_FIELD=m_value]
            [WRITE_FIELD=m_value];
        PROPERTY(ValueU8,      READ_METHOD(prop) WRITE_METHOD(setProp));
        PROPERTY(ValueObject,  READ_FIELD(m_other) WRITE_FIELD(m_other));*/
    END
};

class RTTIEXPORT Test2 : public Test
{
public:
    Test2();
    ~Test2();

    METACLASS(RTTIEXPORT,Test2,Test)
    PROPERTIES
    END
};
}

/*****************************************************************************/
#endif
