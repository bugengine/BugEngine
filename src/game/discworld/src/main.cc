/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>

#include    <main/main.hh>
#include    <system/file/diskfolder.script.hh>
#include    <system/plugin.hh>

int be_main(weak<BugEngine::Application> app)
{
    ref<BugEngine::Folder> dataFolder = ref<BugEngine::DiskFolder>::create(BugEngine::gameArena(), BugEngine::Environment::getEnvironment().getDataDirectory());
    BugEngine::Plugin<minitl::pointer> p1("nullrender", weak<const BugEngine::Folder>(dataFolder));
    //BugEngine::Plugin<minitl::pointer> p2("GL4", weak<const BugEngine::Folder>(dataFolder));

    app->run(dataFolder->openFile(BugEngine::istring("main.pkg")));
    return 0;
}

