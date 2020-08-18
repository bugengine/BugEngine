/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/core/stdafx.h>
#include <bugengine/core/environment.hh>

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

extern BE_IMPORT const char* s_dataDirectory;

namespace BugEngine {

Environment::Environment()
    : m_homeDirectory(s_dataDirectory)
    , m_dataDirectory(ipath("apk:/assets"))
    , m_game("")
    , m_user("android")
{
    be_assert(s_dataDirectory, "Data directory not set when Environment created");
    m_homeDirectory.push_back(".bugengine");
}

Environment::~Environment()
{
}

void Environment::init(int argc, const char* argv[])
{
    m_game               = istring("sample.particlerain");
    const char* filename = argv[0];
    while(*filename != 0)
    {
        filename++;
    }
    while(*filename != '/' && filename != argv[0])
    {
        filename--;
    }
    filename--;
    while(*filename != '/' && filename != argv[0])
    {
        filename--;
    }
    ipath rootdir = ipath(argv[0], filename);
    for(u32 i = 0; i < rootdir.size(); ++i)
    {
        chdir(rootdir[i].c_str());
    }
    for(int arg = 1; arg < argc; arg++)
    {
        if(argv[arg][0] == '-')
        {
            continue;
        }
        m_game = argv[arg];
    }
}

size_t Environment::getProcessorCount() const
{
    return sysconf(_SC_NPROCESSORS_ONLN);
}

const char* Environment::getEnvironmentVariable(const char* variable) const
{
    be_forceuse(variable);
    return NULL;
}

}  // namespace BugEngine
