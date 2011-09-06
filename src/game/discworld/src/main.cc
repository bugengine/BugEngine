/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>

#include    <main/main.hh>
#include    <system/file/folder.script.hh>
#include    <system/plugin.hh>

int be_main(weak<BugEngine::Application> app)
{
    ref<BugEngine::Folder> dataFolder; // = ref<BugEngine::Folder>::create(BugEngine::gameArena());
    BugEngine::Plugin<minitl::pointer> p1("nullrender", weak<const BugEngine::Folder>(dataFolder));
    BugEngine::Plugin<minitl::pointer> p2("GL4", weak<const BugEngine::Folder>(dataFolder));
    app->run();
    return 0;
}

