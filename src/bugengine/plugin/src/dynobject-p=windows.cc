/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin/stdafx.h>
#include <bugengine/core/environment.hh>
#include <bugengine/plugin/dynobject.hh>

#if !BE_STATIC

#    include <winerror.h>

namespace BugEngine { namespace Plugin {

DynamicObject::Handle DynamicObject::load(const inamespace& pluginName, const ipath& pluginPath)
{
    SetLastError(0);
    minitl::format< 1024u > plugingFile = minitl::format< 1024u >("%s.dll") | pluginName;
    const ipath&            pluginDir   = Environment::getEnvironment().getDataDirectory();
    ifilename::Filename fullPath = (pluginDir + pluginPath + ifilename(plugingFile.c_str())).str();
    be_info("loading dynamic object %s (%s)" | pluginName | fullPath.name);
    HANDLE h = LoadLibrary(fullPath.name);
    if(!h)
    {
        char* errorMessage = 0;
        int   errorCode    = ::GetLastError();
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, errorCode,
                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                      reinterpret_cast< LPSTR >(&errorMessage), 0, NULL);
        be_error("Error loading dynamic object %s: %s" | pluginName | errorMessage);
        ::LocalFree(errorMessage);
    }
    return h;
}

void DynamicObject::unload(Handle handle)
{
    FreeLibrary(static_cast< HMODULE >(handle));
}

void* DynamicObject::getSymbolInternal(Handle handle, const istring& name)
{
    return (void*)(GetProcAddress(static_cast< HINSTANCE >(handle), name.c_str()));
}

}}  // namespace BugEngine::Plugin
#endif
