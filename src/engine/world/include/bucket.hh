/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
 see LICENSE for detail */

#ifndef BE_WORLD_BUCKET_HH_
#define BE_WORLD_BUCKET_HH_
/*****************************************************************************/
#include    <world/entitystorage.script.hh>

namespace BugEngine { namespace World
{

struct EntityStorage::Bucket
{
    u64 componentMask;
    u32* indices;

    Bucket();
    ~Bucket();
};

}}


/*****************************************************************************/
#endif
