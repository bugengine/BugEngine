/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_RENDERER_GRAPH_LOADER_HH_
#define BE_GRAPHICS_RENDERER_GRAPH_LOADER_HH_
/*****************************************************************************/
#include    <graphics/objects/scenegraph.script.hh>

namespace BugEngine { namespace Graphics
{

class IRenderer;

class SceneGraphLoader : public minitl::pointer
{
private:
    weak<const IRenderer> m_renderer;
public:
    SceneGraphLoader(weak<const IRenderer> renderer);
    ~SceneGraphLoader();
private:
    ResourceHandle load(weak<const RenderScene> source);
    ResourceHandle load(weak<const RenderSequence> source);
    void  unload(const ResourceHandle& resource);
};

}}

/*****************************************************************************/
#endif
