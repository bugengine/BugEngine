/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

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
    MainScene(weak<const BugEngine::Application> application);
    virtual ~MainScene();
};

}


/*****************************************************************************/
#endif
