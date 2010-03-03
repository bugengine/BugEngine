/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_PHYSICSBULLET_BULLETWORLDMANAGER_H_
#define BE_PHYSICSBULLET_BULLETWORLDMANAGER_H_
/*****************************************************************************/

#include    <physics/iworldmanager.hh>

namespace BugEngine { namespace Physics { namespace Bullet
{

class BulletWorldManager : public IWorldManager
{
public:
    BulletWorldManager();
    ~BulletWorldManager();

    virtual ref<IWorld> createWorld(float3 extents) const override;
public:
    void* operator new(size_t size)                  { return ::operator new(size); }
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
public:
    void  operator delete(void* memory)              { return ::operator delete(memory); }
    void  operator delete(void* memory, void* where) { return ::operator delete(memory, where); }
};

}}}

/*****************************************************************************/
#endif

