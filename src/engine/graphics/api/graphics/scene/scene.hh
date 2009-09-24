/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_SCENE_SCENE_HH_
#define BE_GRAPHICS_SCENE_SCENE_HH_
/*****************************************************************************/
#include    <minitl/ptr/refcountable.hh>
#include    <input/inputmap.hh>

namespace BugEngine { namespace Graphics
{

class be_api(GRAPHICS) Scene : public minitl::refcountable<void>
{
private:
    Scene*                          m_parent;
    std::vector< refptr<Scene> >    m_children;
public:
    Scene(Scene* parent = 0);
    virtual ~Scene();
};

}}


/*****************************************************************************/
#endif
