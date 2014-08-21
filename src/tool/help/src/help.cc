/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>
#include    <help.hh>
#include    <filesystem/diskfolder.script.hh>


namespace BugEngine
{

Help::Help(const Plugin::Context& context)
    :   Application(ref<DiskFolder>::create(Arena::game(), Environment::getEnvironment().getDataDirectory()), context.scheduler)
{
}

Help::~Help()
{
}

}
