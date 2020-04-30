/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin.ui.console/stdafx.h>
#include <uiwidgetloader.hh>
#include <window.script.hh>

#define raw raw2
#include <curses.h>
#undef raw

namespace BugEngine {

UIWidgetLoader::UIWidgetLoader() : ILoader()
{
}

UIWidgetLoader::~UIWidgetLoader()
{
}

void UIWidgetLoader::load(weak< const Resource::Description > description,
                          Resource::Resource&                 resource)
{
    weak< const Window > window = be_checked_cast< const Window >(description);
    resource.setRefHandle(window->create());
}

void UIWidgetLoader::reload(weak< const Resource::Description > oldDescription,
                            weak< const Resource::Description > newDescription,
                            Resource::Resource&                 resource)
{
    be_forceuse(oldDescription);
    weak< const Window > window = be_checked_cast< const Window >(newDescription);
    resource.clearRefHandle();
    resource.setRefHandle(window->create());
}

void UIWidgetLoader::unload(Resource::Resource& resource)
{
    resource.clearRefHandle();
}

}  // namespace BugEngine
