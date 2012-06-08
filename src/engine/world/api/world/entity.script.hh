/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_ENTITY_SCRIPT_HH_
#define BE_WORLD_ENTITY_SCRIPT_HH_
/*****************************************************************************/

namespace BugEngine { namespace World
{

struct be_api(WORLD) Entity
{
    u32 id;


    bool operator ==(const Entity& other) const { return id == other.id; }
    bool operator !=(const Entity& other) const { return id != other.id; }
    bool operator < (const Entity& other) const { return id < other.id; }
};

}}


/*****************************************************************************/
#endif
