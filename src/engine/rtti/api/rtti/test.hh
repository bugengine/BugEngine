/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

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
            [be_set(setValue)];

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
