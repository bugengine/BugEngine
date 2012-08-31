/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_RENDERER_GRAPH_LOADER_HH_
#define BE_GRAPHICS_RENDERER_GRAPH_LOADER_HH_
/*****************************************************************************/
#include    <resource/loader.hh>
#include    <3d/scene/scenegraph.script.hh>

namespace BugEngine
{

class IRenderer;

class SceneGraphLoader : public Resource::ILoader
{
private:
    weak<const IRenderer>   m_renderer;
public:
    SceneGraphLoader(weak<const IRenderer> renderer);
    ~SceneGraphLoader();
private:
    virtual void load(weak<const Resource::Description> description, Resource::Resource& resource) override;
    virtual void unload(Resource::Resource& resource) override;
};

}

/*****************************************************************************/
#endif
