/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/environment.hh>
#include    <unistd.h>
#include    <cerrno>
#include    <cstdio>


namespace BugEngine
{

Environment::Environment()
:   m_homeDirectory(getenv("HOME"))
,   m_dataDirectory(ipath("share/bugengine"))
,   m_game("")
,   m_user(getenv("USER"))
{
    m_homeDirectory.push_back(".bugengine");
}

Environment::~Environment()
{
}

void Environment::init(int argc, const char *argv[])
{
    m_game = istring("sample.text");
    const char* filename = argv[0];
    while (*filename != 0)
    {
        filename++;
    }
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
    for (u32 i = 0; i < rootdir.size(); ++i)
    {
        chdir(rootdir[i].c_str());
    }
    for (int arg = 1; arg < argc; arg++)
    {
        if (argv[arg][0] == '-')
        {
            continue; // TODO
        }
        m_game = argv[arg];
    }
}

}
