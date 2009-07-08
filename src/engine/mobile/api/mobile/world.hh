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
* Boston, MA                                                                  *
* 02110-1301  USA                                                             *
\*****************************************************************************/
    
#ifndef BE_MOBILE_WORLD_HH_
#define BE_MOBILE_WORLD_HH_
/*****************************************************************************/
#include    <graphics/world.hh>
#include    <graphics/scene/scene.hh>
#include    <physics/world.hh>
#include    <sound/world.hh>

namespace BugEngine
{

class Scheduler;

class MOBILEEXPORT World : public Object
{
private:
    refptr<Graphics::World>         m_graphicsSystem;
    refptr<Physics::World>          m_physicsSystem;
    refptr<Sound::World>            m_soundSystem;
public:
    World(float3 worldExtents);
    ~World();

    void createView(Graphics::WindowFlags f, refptr<Graphics::Scene> scene);

    BaseTask* getStartWorldUpdate();
    BaseTask* getEndWorldUpdate();

    be_metaclass(MOBILEEXPORT,World,Object)
    be_properties
    be_end
};

}


/*****************************************************************************/
#endif
