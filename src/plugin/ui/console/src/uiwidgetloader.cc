/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <console/stdafx.h>
#include    <uiwidgetloader.hh>
#include    <window.script.hh>

#define raw raw2
#include    <curses.h>
#undef raw

namespace BugEngine
{

UIWidgetLoader::UIWidgetLoader()
    :   ILoader()
{
}

UIWidgetLoader::~UIWidgetLoader()
{
}

void UIWidgetLoader::load(weak<const Resource::Description> description, Resource::Resource& resource)
{
    weak<const Window> window = be_checked_cast<const Window>(description);
    be_info("loading window \"%s\"" | window->name);
    be_forceuse(resource);
}

void UIWidgetLoader::reload(weak<const Resource::Description> oldDescription,
                            weak<const Resource::Description> newDescription,
                            Resource::Resource& resource)
{
    weak<const Window> window = be_checked_cast<const Window>(oldDescription);
    be_info("reloading window \"%s\"" | window->name);
    be_forceuse(oldDescription);
    be_forceuse(newDescription);
    be_forceuse(resource);
}

void UIWidgetLoader::unload(Resource::Resource& resource)
{
    be_info("unload");
    be_forceuse(resource);
}

}
