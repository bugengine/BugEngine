/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>
#include    <storage.script.hh>
#include    <scheduler/kernel/stream.factory.hh>

namespace BugEngine
{

void A::created(World::World& /*world*/)
{
    be_info("created A (%d)" | value);
}
void A::destroyed(World::World& /*world*/)
{
    be_info("destroyed A (%d)" | value);
}

void B::created(World::World& /*world*/)
{
    be_info("created B (%d)" | value);
}
void B::destroyed(World::World& /*world*/)
{
    be_info("destroyed B (%d)" | value);
}

void C::created(World::World& /*world*/)
{
    be_info("created C (%d)" | value);
}
void C::destroyed(World::World& /*world*/)
{
    be_info("destroyed C (%d)" | value);
}

void D::created(World::World& /*world*/)
{
    be_info("created D (%d)" | value);
}
void D::destroyed(World::World& /*world*/)
{
    be_info("destroyed D (%d)" | value);
}

void E::created(World::World& /*world*/)
{
    be_info("created E (%d)" | value);
}
void E::destroyed(World::World& /*world*/)
{
    be_info("destroyed E (%d)" | value);
}

KernelStorage::KernelStorage()
{
}

KernelStorage::~KernelStorage()
{
}

}
