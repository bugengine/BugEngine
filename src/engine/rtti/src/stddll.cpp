#include    <rtti/stdafx.h>

#if defined(BE_PLATFORM_WIN32) && defined(BUILDING_DLL)

namespace BugEngine
{
    HINSTANCE hDllInstance;
}

extern "C" BOOL WINAPI DllMain( HINSTANCE hinstDLL, 
                                DWORD fdwReason,
                                LPVOID /*lpvReserved*/)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        BugEngine::hDllInstance = hinstDLL; 
    }
    return TRUE;
}

#endif
