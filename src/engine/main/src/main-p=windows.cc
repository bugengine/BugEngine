/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <bugengine/stdafx.h>
#include    <windows.h>
#include    <iostream>
#include    <io.h>
#include    <stdio.h>
#include    <fcntl.h>
#include    <fstream>

int beMain(int argc, const char *argv[]);

extern "C"
int WINAPI WinMain( HINSTANCE /*hInstance*/,
                    HINSTANCE /*hPrevInstance*/,
                    LPSTR /*lpCmdLine*/,
                    int /*nCmdShow*/ )
{
    AttachConsole(ATTACH_PARENT_PROCESS);
    //AllocConsole();
    if (GetConsoleWindow())
    {
        long stdHandle = static_cast<long>((uintptr_t)GetStdHandle(STD_OUTPUT_HANDLE));
        int conHandle = _open_osfhandle(stdHandle, _O_TEXT);
        FILE* fp = _fdopen(conHandle, "w");
        *stdout = *fp;
        setvbuf(stdout, NULL, _IONBF, 0);

        stdHandle = static_cast<long>((uintptr_t)GetStdHandle(STD_INPUT_HANDLE));
        conHandle = _open_osfhandle(stdHandle, _O_TEXT);
        fp = _fdopen( conHandle, "r" );
        *stdin = *fp;
        setvbuf(stdin, NULL, _IONBF, 0);

        stdHandle = static_cast<long>((uintptr_t)GetStdHandle(STD_ERROR_HANDLE));
        conHandle = _open_osfhandle(stdHandle, _O_TEXT);
        fp = _fdopen( conHandle, "w" );
        *stderr = *fp;
        setvbuf(stderr, NULL, _IONBF, 0);

        std::ios::sync_with_stdio();
    }
    return beMain(__argc, (const char **)__argv);
}
