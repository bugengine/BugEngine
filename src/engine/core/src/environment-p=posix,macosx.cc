/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/environment.hh>
#include    <unistd.h>
#include    <cerrno>
#include    <cstdlib>
#include    <cstdio>


namespace BugEngine
{

Environment::Environment()
:   m_homeDirectory(getenv("HOME"))
,   m_dataDirectory(ipath("share/bugengine"))
,   m_game("")
,   m_user(getenv("USER"))
,   m_programPath(0)
{
    m_homeDirectory.push_back(".bugengine");
}

Environment::~Environment()
{
    Arena::general().free(m_programPath);
}

void Environment::init(int argc, const char *argv[])
{
    m_game = istring("sample.text");
    const char* filename = argv[0];
    while (*filename != 0)
    {
        filename++;
    }
    size_t arglen = filename - argv[0] + 2;
    char* buffer = 0;
    char* dest = 0;
    if (argv[0][0] != '/')
    {
        size_t extraLen = 0;
        do
        {
            extraLen += 256;
            buffer = (char*)Arena::general().realloc(buffer, arglen + extraLen, 4);
            dest = getcwd(buffer, extraLen);
        } while (dest == 0 && errno == ERANGE);
        dest += strlen(dest);
        if (dest[-1] != '/')
        {
            *dest = '/';
            dest++;
        }
    }
    else
    {
        buffer = (char*)Arena::general().alloc(arglen, 4);
        dest = buffer;
    }
    strncpy(dest, argv[0], arglen);
    m_programPath = buffer;

    while (*filename != '/' && filename != argv[0])
    {
        filename--;
    }
    filename--;
    while (*filename != '/' && filename != argv[0])
    {
        filename--;
    }
    ipath rootdir = ipath(argv[0], filename);
    chdir(rootdir.str().name);
    m_dataDirectory = m_dataDirectory;
    for (int arg = 1; arg < argc; arg++)
    {
        if (argv[arg][0] == '-')
        {
            continue; // TODO
        }
        m_game = argv[arg];
    }
}

const char* Environment::getEnvironmentVariable(const char *variable) const
{
    return getenv(variable);
}

}
