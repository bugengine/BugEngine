/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_TEXT_TEXTMANAGER_HH_
#define BE_TEXT_TEXTMANAGER_HH_
/**************************************************************************************************/
#include    <text/stdafx.h>
#include    <resource/loader.hh>

namespace BugEngine
{

class FontList;

class TextManager : public Resource::ILoader
{
public:
    TextManager();
    ~TextManager();

    void load(weak<const Resource::Description> description,
              Resource::Resource& resource) override;
    void reload(weak<const Resource::Description> oldDescription,
                weak<const Resource::Description> newDescription,
                Resource::Resource& resource) override;
    void unload(Resource::Resource& resource) override;
};

}

/**************************************************************************************************/
#endif
