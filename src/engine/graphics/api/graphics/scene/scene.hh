/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_SCENE_SCENE_HH_
#define BE_GRAPHICS_SCENE_SCENE_HH_
/*****************************************************************************/
#include    <minitl/ptr/refcountable.hh>
#include    <input/inputmap.hh>

namespace BugEngine { namespace Graphics
{

class be_api(GRAPHICS) Scene : public minitl::refcountable
{
private:
    weak<Scene>                  m_parent;
    std::vector< ref<Scene> >    m_children;
public:
    Scene(weak<Scene> parent = weak<Scene>());
    virtual ~Scene();
};

}}


/*****************************************************************************/
#endif
