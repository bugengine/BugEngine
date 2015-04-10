/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/environment.hh>
#include    <userenv.h>
#include    <stdlib.h>

typedef BOOL (WINAPI *GetUserProfileDirectoryFunction)(HANDLE hToken, LPSTR lpProfileDir, LPDWORD lpcchSize);

namespace BugEngine
{

Environment::Environment()
:   m_homeDirectory("")
,   m_dataDirectory("")
,   m_game("")
,   m_user("")
,   m_programPath(0)
{
    HANDLE token;
    OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token);
    char profile[MAX_PATH];
    DWORD size = sizeof(profile);
    GetUserName(profile, &size);
    m_user = profile;
    size = sizeof(profile);
    HMODULE h = LoadLibraryA("userenv.dll");
    if (h != 0)
    {
        GetUserProfileDirectoryFunction function = (GetUserProfileDirectoryFunction)GetProcAddress(h, "GetUserProfileDirectoryA");
        (*function)(token, profile, &size);
        FreeLibrary(h);
    }
    m_homeDirectory = profile;
    m_homeDirectory.push_back(istring("bugengine"));
}

Environment::~Environment()
{
    SetDllDirectoryA(NULL);
}

extern "C" IMAGE_DOS_HEADER __ImageBase;
void Environment::init()
{
    char dllPath[MAX_PATH] = {0};
    GetModuleFileNameA((HINSTANCE)&__ImageBase, dllPath, sizeof(dllPath));
    const char* progName = dllPath;
    init(1, &progName);
}

void Environment::init(int argc, const char *argv[])
{
    m_game = istring("sample.text");
    ipath rootPath = canonicalPath(argv[0], "\\/");
    m_programPath = ifilename(rootPath);
    rootPath.pop_back();
    m_dataDirectory = rootPath + m_dataDirectory;

    for (int arg = 1; arg < argc; arg++)
    {
        if (argv[arg][0] == '-')
        {
            continue; // TODO
        }
        m_game = argv[arg];
    }

    SetDllDirectoryA((getDataDirectory()+ipath("plugin")).str().name);
}

size_t Environment::getProcessorCount() const
{
    SYSTEM_INFO i;
    GetSystemInfo(&i);
    return i.dwNumberOfProcessors;
}

const char* Environment::getEnvironmentVariable(const char *variable) const
{
    return getenv(variable);
}

}
