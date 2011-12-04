/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <editor.hh>

namespace BugEngine { namespace Editor
{

Editor::Editor(weak<Application> application)
    :   m_application(application)
{
}

Editor::~Editor()
{
}

}}
