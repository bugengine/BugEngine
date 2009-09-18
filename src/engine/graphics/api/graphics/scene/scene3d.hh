/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_SCENE_SCENE3D_HH_
#define BE_GRAPHICS_SCENE_SCENE3D_HH_
/*****************************************************************************/
#include    <graphics/scene/scene.hh>

namespace BugEngine { namespace Graphics
{

class GRAPHICSEXPORT Scene3D : public Scene
{
public:
    Scene3D(Scene* parent);
    virtual ~Scene3D();
};

}}


/*****************************************************************************/
#endif
