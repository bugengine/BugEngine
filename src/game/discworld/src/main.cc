/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>

#include    <main/main.hh>
#include    <system/file/filesystem.hh>
#include    <system/plugin.hh>

int be_main(weak<BugEngine::Application> app)
{
    ref<BugEngine::FileSystem> fs = ref<BugEngine::FileSystem>::create(BugEngine::gameArena());
    BugEngine::Plugin<minitl::pointer> plugin("GL4", weak<const BugEngine::FileSystem>(fs));
    app->run();
    return 0;
}
