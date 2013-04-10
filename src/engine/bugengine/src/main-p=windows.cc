/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <bugengine/stdafx.h>
#include    <windows.h>

int beMain(int argc, const char *argv[]);

extern "C"
int WINAPI WinMain( HINSTANCE /*hInstance*/,
                    HINSTANCE /*hPrevInstance*/,
                    LPSTR /*lpCmdLine*/,
                    int /*nCmdShow*/ )
{
    return beMain(__argc, (const char **)__argv);
}
