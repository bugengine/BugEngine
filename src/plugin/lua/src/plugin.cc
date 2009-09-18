/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>

#include    <context.h>


static BugEngine::Lua::Context* s_context = 0;

extern "C" FORCEEXPORT void _initplugin(void)
{
    s_context = new BugEngine::Lua::Context;
}

extern "C" FORCEEXPORT void _finiplugin(void)
{
    delete s_context;
    s_context = 0;
}

extern "C" FORCEEXPORT void doFile(const char *filename)
{
    s_context->doFile(filename);
}
