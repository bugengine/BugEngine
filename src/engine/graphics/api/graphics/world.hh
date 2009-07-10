/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

#ifndef BE_GRAPHICS_WORLD_HH_
#define BE_GRAPHICS_WORLD_HH_
/*****************************************************************************/
#include    <graphics/renderer/renderer.hh>
#include    <graphics/renderer/rendertarget.hh>
#include    <graphics/scene/scene.hh>
#include    <system/scheduler/task.hh>

namespace BugEngine
{
class Scheduler;
}

namespace BugEngine { namespace Graphics
{

class GRAPHICSEXPORT World : public Object
{
private:
    class UpdateWindowManagement;
private:
    typedef std::vector< refptr<RenderTarget> > SceneList;
    scopedptr<Renderer>             m_renderer;
    SceneList                       m_scenes;
    refptr< BaseTask >              m_updateWindowTask;
    refptr< BaseTask >              m_flushTask;
private:
    int step();
    void flush();
public:
    World();
    ~World();

    void createWindow(WindowFlags f, refptr<Scene> scene);

    be_metaclass(GRAPHICSEXPORT,World, Object)
    be_properties
    be_end
};

}}

/*****************************************************************************/
#endif
