/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_SAMPLES_PYTHON_FUNCTIONS_SCRIPT_HH_
#define BE_SAMPLES_PYTHON_FUNCTIONS_SCRIPT_HH_
/**************************************************************************************************/
#include    <stdafx.h>

namespace BugEngine { namespace TestCases
{

class Class : public minitl::refcountable
{
published:
    const int x1;
    const int y1;

    Class(u32 x1=0, u32 y1=0) : x1(x1), y1(y1) {}
    ~Class() {}

    void doStuff(float v1, float v2 = 5.0f);
    void doStuff(u32 v1, u32 v2, bool done);
    void doStuff(u32 v1, u32 v2 = 5, u32 v3 = 7);
};

struct Struct
{

};

enum Enum
{
    Value1,
    Value2,
    Value3 = 5
};


}}

/**************************************************************************************************/
#endif
