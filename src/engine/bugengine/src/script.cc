/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <bugengine/stdafx.h>
#include    <bugengine/script.script.hh>

namespace BugEngine
{

Script::Script(weak<const File> file)
    :   m_file(file)
{
}

Script::~Script()
{
}

}

