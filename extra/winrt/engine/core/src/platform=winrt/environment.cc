/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <core/stdafx.h>
#include    <core/environment.hh>
#include    <userenv.h>
#include    <windows.h>

namespace BugEngine
{

Environment::Environment()
:   m_homeDirectory("")
,   m_dataDirectory("")
,   m_game("")
,   m_user("")
{
}

Environment::~Environment()
{
}

void Environment::init(int argc, const char *argv[])
{
    const char *exe = argv[0];
    size_t s = strlen(argv[0])-1;
    m_game = istring("bugeditor.main");
    do
    {
        if (exe[s] == '\\' || exe[s] == '/')
            break;
        s--;
    }
    while (s != 0);
    size_t begin = s;
    while (exe[s] && exe[s] != '.')
        s++;
    m_dataDirectory = ipath(exe, exe+begin);
    m_dataDirectory.push_back(istring("data"));
    for( int arg = 1; arg < argc; arg++ )
    {
        if (argv[arg][0] == '-')
        {
            continue; // TODO
        }
        m_game = argv[arg];
    }
}

size_t Environment::getProcessorCount() const
{
    return 1;
}

}
