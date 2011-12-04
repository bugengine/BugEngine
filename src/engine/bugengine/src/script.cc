/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <main/stdafx.h>
#include    <main/script.script.hh>

namespace BugEngine
{

Script::Script(ref<const File> file)
    :   m_file(file)
{
}

Script::~Script()
{
}

}