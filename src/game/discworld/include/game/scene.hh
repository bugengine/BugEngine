/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef DISCWORLD_GAME_SCENE_HH_
#define DISCWORLD_GAME_SCENE_HH_
/*****************************************************************************/
#include    <graphics/scene/scene3d.hh>

namespace Discworld
{

class GameScene : public BugEngine::Graphics::Scene3D
{
public:
    GameScene(BugEngine::Graphics::Scene* scene);
    virtual ~GameScene();
};

}

/*****************************************************************************/
#endif
