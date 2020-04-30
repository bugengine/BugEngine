/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_UI_CONSOLE_UIWIDGETLOADER_HH_
#define BE_UI_CONSOLE_UIWIDGETLOADER_HH_
/**************************************************************************************************/
#include <bugengine/resource/loader.hh>

namespace BugEngine {

class UIWidgetLoader : public Resource::ILoader
{
public:
    UIWidgetLoader();
    ~UIWidgetLoader();

    virtual void load(weak< const Resource::Description > script,
                      Resource::Resource&                 resource) override;
    virtual void reload(weak< const Resource::Description > oldScript,
                        weak< const Resource::Description > newScript,
                        Resource::Resource&                 resource) override;
    virtual void unload(Resource::Resource& resource) override;
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
