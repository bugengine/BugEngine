/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_SCENE_SCENE3D_HH_
#define BE_GRAPHICS_SCENE_SCENE3D_HH_
/*****************************************************************************/
#include    <graphics/scene/scene.hh>
#include    <graphics/camera/camera.hh>

namespace BugEngine { namespace Graphics
{

class be_api(GRAPHICS) Scene3D : public Scene
{
private:
    ref<Camera> m_camera;
public:
    Scene3D();
    virtual ~Scene3D();
};

}}


/*****************************************************************************/
#endif
