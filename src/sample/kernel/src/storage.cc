/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <stdafx.h>
#include    <storage.script.hh>
#include    <scheduler/kernel/stream.hh>

namespace BugEngine
{

void A::created()
{
    be_info("created A (%d)" | value);
}
void A::destroyed()
{
    be_info("destroyed A (%d)" | value);
}

void B::created()
{
    be_info("created B (%d)" | value);
}
void B::destroyed()
{
    be_info("destroyed B (%d)" | value);
}

void C::created()
{
    be_info("created C (%d)" | value);
}
void C::destroyed()
{
    be_info("destroyed C (%d)" | value);
}

void D::created()
{
    be_info("created D (%d)" | value);
}
void D::destroyed()
{
    be_info("destroyed D (%d)" | value);
}

void E::created()
{
    be_info("created E (%d)" | value);
}
void E::destroyed()
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
