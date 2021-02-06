/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <stdafx.h>
#include <bugengine/main.hh>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <windows.h>

#ifdef _CONSOLE
extern "C" int main(int argc, const char* argv[])
{
    return beMain(argc, argv);
}
#else
extern "C" int WINAPI WinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/,
                              LPSTR /*lpCmdLine*/, int /*nCmdShow*/)
{
    AttachConsole(ATTACH_PARENT_PROCESS);
    // AllocConsole();
    if(GetConsoleWindow())
    {
        long  stdHandle = static_cast< long >((uintptr_t)GetStdHandle(STD_OUTPUT_HANDLE));
        int   conHandle = _open_osfhandle(stdHandle, _O_TEXT);
        FILE* fp        = _fdopen(conHandle, "w");
        *stdout         = *fp;
        setvbuf(stdout, NULL, _IONBF, 0);

        stdHandle = static_cast< long >((uintptr_t)GetStdHandle(STD_INPUT_HANDLE));
        conHandle = _open_osfhandle(stdHandle, _O_TEXT);
        fp        = _fdopen(conHandle, "r");
        *stdin    = *fp;
        setvbuf(stdin, NULL, _IONBF, 0);

        stdHandle = static_cast< long >((uintptr_t)GetStdHandle(STD_ERROR_HANDLE));
        conHandle = _open_osfhandle(stdHandle, _O_TEXT);
        fp        = _fdopen(conHandle, "w");
        *stderr   = *fp;
        setvbuf(stderr, NULL, _IONBF, 0);
    }
    return beMain(__argc, (const char**)__argv);
}
#endif
