/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>

#include    <context.h>


extern "C" FORCEEXPORT BugEngine::Lua::Context* be_createPlugin(void)
{
    return new BugEngine::Lua::Context;
}

extern "C" FORCEEXPORT void be_destroyPlugin(BugEngine::Lua::Context* context)
{
    delete context;
}
