/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <plugin/plugin.hh>
#include    <core/environment.hh>
#include    <winerror.h>

namespace BugEngine
{

DynamicObject::Handle DynamicObject::load(const inamespace &pluginName, const ipath& pluginPath)
{
    SetLastError(0);
    minitl::format<1024u> plugingFile = minitl::format<1024u>("%s.dll") | pluginName;
    const ipath& pluginDir = Environment::getEnvironment().getDataDirectory();
    ifilename::Filename pluginPath = (pluginDir + pluginPath + ifilename(plugingFile.c_str())).str();
    be_info("loading plugin %s (%s)" | pluginName | pluginPath.name);
    HANDLE h = LoadLibrary(pluginPath.name);
    if (!h)
    {
        char *errorMessage = 0;
        int errorCode = ::GetLastError();
        FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            errorCode,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            reinterpret_cast<LPSTR>(&errorMessage),
            0,
            NULL);
        be_error("%s : %s"|pluginName|errorMessage);
        ::LocalFree(errorMessage)
    }
    return h;
}

void DynamicObject::unload(Handle handle)
{
    FreeLibrary(static_cast<HMODULE>(handle));
}

void* DynamicObject::getSymbolInternal(Handle handle, const char* name)
{
    return GetProcAddress(static_cast<HINSTANCE>(handle), name);
}


}

