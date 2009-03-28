#include    <vge/core/stdafx.h>

#if defined(WIN32) && defined(_DLL)
#include <windows.h>

namespace VGE
{
    HINSTANCE hDllInstance;
}

extern "C" BOOL WINAPI DllMain( HINSTANCE hinstDLL, 
                                DWORD fdwReason,
                                LPVOID /*lpvReserved*/)
{
    if( fdwReason == DLL_PROCESS_ATTACH)
    {
        VGE::hDllInstance = hinstDLL; 
    }
    return TRUE;
}

#endif
