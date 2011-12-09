/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <editor.hh>
#include    <system/plugin.hh>
#include    <bugengine/application.hh>

BE_PLUGIN_REGISTER(bugeditor, minitl::refcountable, BugEngine::Editor::Editor, (weak<BugEngine::Application> application), (application));
