#include    <core/stdafx.h>
#include    <test.hh>

#ifdef _WIN32
namespace BugEngine
{
    HINSTANCE hDllInstance = 0;
}
#endif

int do_test()
{
    i_u32 counter = 0;
    BE_TEST(++counter == 1);
    BE_TEST(counter++ == 1);
    BE_TEST(--counter == 1);
    BE_TEST(counter-- == 1);
    return EXIT_SUCCESS;
}
