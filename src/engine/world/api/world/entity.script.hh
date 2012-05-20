/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_ENTITY_SCRIPT_HH_
#define BE_WORLD_ENTITY_SCRIPT_HH_
/*****************************************************************************/

namespace BugEngine { namespace World
{

union be_api(WORLD) Entity
{
published:
    struct Index
    {
        u16 world;
        u16 block;
        u32 offset;
    };
published:
    Index   index;
    u64     id;


    bool operator ==(const Entity& other) const { return id == other.id; }
    bool operator !=(const Entity& other) const { return id != other.id; }
    bool operator < (const Entity& other) const { return id < other.id; }
};

}}


/*****************************************************************************/
#endif
