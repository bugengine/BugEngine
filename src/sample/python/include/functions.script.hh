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

    Class(u32 x1, u32 y1) : x1(x1), y1(y1) {}
    ~Class() {}
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
