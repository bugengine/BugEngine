/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/core/stdafx.h>
#include <bugengine/core/environment.hh>

#include <cstdio>
#include <cstdlib>
#include <dlfcn.h>
#include <unistd.h>

namespace BugEngine {

BE_EXPORT void* s_dummyData = 0;

Environment::Environment()
    : m_homeDirectory(getenv("HOME"))
    , m_dataDirectory("data")
    , m_game("")
    , m_user(getenv("USER") ? getenv("USER") : "")
    , m_programPath()
{
    m_homeDirectory.push_back(".bugengine");
}

void Environment::init()
{
    Dl_info info;
    dladdr(&s_dummyData, &info);
    init(1, &info.dli_fname);
}

void Environment::init(int argc, const char* argv[])
{
    m_game         = istring("sample.lua");
    ipath rootPath = canonicalPath(argv[0], "/");
    m_programPath  = ifilename(rootPath);
    rootPath.pop_back();
    m_dataDirectory = rootPath + m_dataDirectory;

    for(int arg = 1; arg < argc; arg++)
    {
        if(argv[arg][0] == '-')
        {
            continue;
        }
        m_game = argv[arg];
    }
}

Environment::~Environment()
{
}

size_t Environment::getProcessorCount() const
{
    return sysconf(_SC_NPROCESSORS_ONLN);
}

const char* Environment::getEnvironmentVariable(const char* variable) const
{
    return getenv(variable);
}

}  // namespace BugEngine
