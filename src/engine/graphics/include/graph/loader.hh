/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_RENDERER_GRAPH_LOADER_HH_
#define BE_GRAPHICS_RENDERER_GRAPH_LOADER_HH_
/*****************************************************************************/
#include    <system/resource/iresourceloader.script.hh>

namespace BugEngine { namespace Graphics
{

class SceneGraphLoader : public IResourceLoader
{
public:
    SceneGraphLoader();
    ~SceneGraphLoader();
private:
    virtual void* load(weak<const Resource> source) override;
    virtual void  unload(const void* resource) override;
};

}}

/*****************************************************************************/
#endif
