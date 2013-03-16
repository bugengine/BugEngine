/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
 see LICENSE for detail */

#include    <world/stdafx.h>

#include    <bucket.hh>
#include    <componentgroup.hh>

namespace BugEngine { namespace World
{

EntityStorage::ComponentGroup::ComponentGroup()
:   m_bucket(Arena::game(), 0)
{
}

EntityStorage::ComponentGroup::~ComponentGroup()
{
}

}}
