/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_TEXT_TEXTMANAGER_HH_
#define BE_TEXT_TEXTMANAGER_HH_
/**************************************************************************************************/
#include <bugengine/plugin.graphics.text/stdafx.h>
#include <bugengine/resource/loader.hh>

namespace BugEngine {

class FontList;

class TextManager : public Resource::ILoader
{
public:
    TextManager();
    ~TextManager();

    void load(weak< const Resource::Description > description,
              Resource::Resource&                 resource) override;
    void reload(weak< const Resource::Description > oldDescription,
                weak< const Resource::Description > newDescription,
                Resource::Resource&                 resource) override;
    void unload(Resource::Resource& resource) override;
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
