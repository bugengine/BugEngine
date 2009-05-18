#include    <input/stdafx.h>

#if defined(WIN32) && defined(BUILDING_DLL)
#include <core/memory/new.inl>

namespace BugEngine
{
    HINSTANCE hDllInstance;
}

extern "C" BOOL WINAPI DllMain( HINSTANCE hinstDLL, 
                                DWORD fdwReason,
                                LPVOID /*lpvReserved*/)
{
    if( fdwReason == DLL_PROCESS_ATTACH)
    {
        BugEngine::hDllInstance = hinstDLL; 
    }
    return TRUE;
}

#endif
