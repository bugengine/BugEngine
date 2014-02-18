/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_TEXT_BITMAPFONTMANAGER_HH_
#define BE_TEXT_BITMAPFONTMANAGER_HH_
/*****************************************************************************/
#include    <resource/loader.hh>

namespace BugEngine
{

namespace Resource
{
struct Resource;
class Description;
}
class FontList;

class BitmapFontManager : public Resource::ILoader
{
private:
    weak<const FontList>    m_fontList;
public:
    BitmapFontManager(weak<const FontList> fontList);
    ~BitmapFontManager();

    void load(weak<const Resource::Description> description, Resource::Resource& resource) override;
    void reload(weak<const Resource::Description> oldDescription, weak<const Resource::Description> newDescription, Resource::Resource& resource) override;
    void unload(Resource::Resource& resource) override;
    void onTicketLoaded(weak<const Resource::Description> /*description*/,
                        Resource::Resource& /*resource*/,
                        const minitl::Allocator::Block<u8>& /*buffer*/,
                        LoadType /*type*/) override;
};

}

/*****************************************************************************/
#endif
