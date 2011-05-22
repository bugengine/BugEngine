/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_GRAPHICS_SCENE_SCENE_INL_
#define BE_GRAPHICS_SCENE_SCENE_INL_
/*****************************************************************************/
#include    <system/scheduler/task/group.hh>
#include    <system/scheduler/range/onestep.hh>
#include    <graphics/renderer/irenderer.hh>

namespace BugEngine { namespace Graphics
{

template< class SpacePartitioning >
typename Scene<SpacePartitioning>::Node Scene<SpacePartitioning>::root()
{
    return Node();
}

}}

/*****************************************************************************/
#endif
