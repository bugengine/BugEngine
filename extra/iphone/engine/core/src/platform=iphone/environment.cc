/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/environment.hh>
#include    <unistd.h>
#include    <sys/types.h>
#include    <sys/sysctl.h>
#include    <errno.h>

namespace BugEngine
{

Environment::Environment()
:   m_homeDirectory(getenv("HOME"))
,   m_dataDirectory("share/bugengine")
,   m_game("sample.kernel")
,   m_user(getenv("USER"))
{
    m_homeDirectory.push_back(".bugengine");
}

void Environment::init(int argc, const char *argv[])
{
    char* path = strdup(argv[0]);
    char* filename = path;
    char* lastSlash = path;
    for (filename = path; *filename; ++filename)
    {
        if (*filename == '/')
            lastSlash = filename;
    }
    *lastSlash = 0;
    filename = lastSlash + 1;
    chdir(path);
    for( int arg = 1; arg < argc; arg++ )
    {
        if (argv[arg][0] == '-')
        {
            continue; // TODO
        }
        m_game = argv[arg];
    }
    m_dataDirectory = ipath(argv[0], filename);
    m_dataDirectory += "share";
    m_dataDirectory += "bugengine";
    free(path);
}

Environment::~Environment()
{
}

size_t Environment::getProcessorCount() const
{
    return sysconf(_SC_NPROCESSORS_ONLN);
}

}
