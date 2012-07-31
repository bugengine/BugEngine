/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <bugengine/stdafx.h>
#include    <windows.h>

int beMain(int argc, const char *argv[]);

namespace BugEngine
{
    HINSTANCE hDllInstance;
}

extern "C"
int WINAPI WinMain( HINSTANCE hInstance,
                    HINSTANCE /*hPrevInstance*/,
                    LPSTR /*lpCmdLine*/,
                    int /*nCmdShow*/ )
{
    BugEngine::hDllInstance = hInstance;
    return beMain(__argc, (const char **)__argv);
}
