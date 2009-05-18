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

#ifndef DISCWORLD_MAINSCENE_HH_
#define DISCWORLD_MAINSCENE_HH_
/*****************************************************************************/
#include    <graphics/scene/scene.hh>
#include    <graphics/renderer/renderer.hh>
#include    <input/action.hh>
#include    <main/application.hh>

namespace Discworld
{

class MainScene : public BugEngine::Graphics::Scene
{
public:
    MainScene(const BugEngine::Application* application);
    virtual ~MainScene();
};

}


/*****************************************************************************/
#endif
