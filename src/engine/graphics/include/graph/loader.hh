/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_RENDERER_GRAPH_LOADER_HH_
#define BE_GRAPHICS_RENDERER_GRAPH_LOADER_HH_
/*****************************************************************************/
#include    <graphics/objects/scenegraph.script.hh>

namespace BugEngine { namespace Graphics
{

class SceneGraphLoader : public minitl::pointer
{
public:
    SceneGraphLoader();
    ~SceneGraphLoader();
private:
    void* load(weak<const SceneGraph> source);
    void  unload(const void* resource);
};

}}

/*****************************************************************************/
#endif
