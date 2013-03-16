/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
 see LICENSE for detail */

#ifndef BE_WORLD_COMPONENTGROUP_HH_
#define BE_WORLD_COMPONENTGROUP_HH_
/*****************************************************************************/
#include    <world/entitystorage.script.hh>

namespace BugEngine { namespace World
{

struct EntityStorage::ComponentGroup
{
    u64 componentMask;
    minitl::array<EntityStorage::Bucket> m_bucket;

    ComponentGroup();
    ~ComponentGroup();
};

}}


/*****************************************************************************/
#endif
