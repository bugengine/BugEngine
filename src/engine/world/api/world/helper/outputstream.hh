/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
 see LICENSE for detail */

#ifndef BE_WORLD_HELPER_OUTPUTSTREAM_HH_
#define BE_WORLD_HELPER_OUTPUTSTREAM_HH_
/**************************************************************************************************/
#include    <world/stdafx.h>
#include    <scheduler/kernel/product.factory.hh>

namespace BugEngine { namespace World
{

template< typename T >
struct OutputStream
{
    scoped< Kernel::Stream<T> > stream;
    const Kernel::Product<T>    product;
    OutputStream(weak<Task::ITask> producer)
        :   stream(scoped< Kernel::Stream<T> >::create(Arena::task()))
        ,   product(stream, producer)
    {
    }
private:
    OutputStream(const OutputStream& other);
    OutputStream& operator=(const OutputStream& other);
};

}}


/**************************************************************************************************/
#endif
