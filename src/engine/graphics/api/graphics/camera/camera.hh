/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_CAMERA_CAMERA_HH_
#define BE_GRAPHICS_CAMERA_CAMERA_HH_
/*****************************************************************************/

namespace BugEngine { namespace Graphics
{

class be_api(GRAPHICS) Camera : public minitl::refcountable
{
protected:
    float4  m_camera;
public:
    Camera();
    virtual ~Camera() { }
};

}}


/*****************************************************************************/
#endif
