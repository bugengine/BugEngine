/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>

#include    <main/main.hh>
#include    <system/file/diskfolder.script.hh>
#include    <system/plugin.hh>

using namespace BugEngine;

int be_main(weak<Application> app)
{
    ref<Folder> dataFolder = ref<DiskFolder>::create(gameArena(), Environment::getEnvironment().getDataDirectory());
    Plugin<minitl::pointer> p1("nullrender", weak<const Folder>(dataFolder));
    //Plugin<minitl::pointer> p2("GL4", weak<const Folder>(dataFolder));

    app->run(dataFolder->openFile(BugEngine::istring("main.pkg")));
    return 0;
}

