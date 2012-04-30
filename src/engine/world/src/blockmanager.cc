/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <world/stdafx.h>
#include    <blockmanager.hh>

namespace BugEngine { namespace World
{

static void* poolAlloc(minitl::itaggedptr<void>& freeBlocks, size_t /*size*/)
{
    minitl::itaggedptr< void >::ticket_t ticket;
    void* result = 0;
    do
    {
        ticket = freeBlocks.getTicket();
        result = freeBlocks;
    }
    while (result && !freeBlocks.setConditional(*(void**)result, ticket));
    if (result)
    {
        return result;
    }
    else
    {
        /*TODO*/
        return result;
    }
}

static void poolFree(minitl::itaggedptr<void>& freeBlocks, void* ptr)
{
    minitl::itaggedptr< void >::ticket_t ticket;
    do
    {
        ticket = freeBlocks.getTicket();
        *(void**)ptr = freeBlocks;
    } while(!freeBlocks.setConditional(ptr, ticket));
}

BlockManager::BlockManager()
:   m_pool256k(0)
,   m_pool128k(0)
,   m_pool64k(0)
,   m_pool32k(0)
,   m_pool16k(0)
,   m_usedBlocks(0)
{
}

BlockManager::~BlockManager()
{
    be_assert(m_usedBlocks == 0, "not all memory reclaimed when the block manager was destroyed");
}

template< > void BlockManager::free<256>(void* ptr)
{
    poolFree(m_pool256k, ptr);
}

template< > void BlockManager::free<128>(void* ptr)
{
    poolFree(m_pool128k, ptr);
}

template< > void BlockManager::free<64>(void* ptr)
{
    poolFree(m_pool64k, ptr);
}

template< > void BlockManager::free<32>(void* ptr)
{
    poolFree(m_pool32k, ptr);
}

template< > void BlockManager::free<16>(void* ptr)
{
    poolFree(m_pool16k, ptr);
}

template< > void* BlockManager::alloc<256>()
{
    return poolAlloc(m_pool256k, 256);
}

template< > void* BlockManager::alloc<128>()
{
    return poolAlloc(m_pool128k, 128);
}

template< > void* BlockManager::alloc<64>()
{
    return poolAlloc(m_pool64k, 64);
}

template< > void* BlockManager::alloc<32>()
{
    return poolAlloc(m_pool32k, 32);
}

template< > void* BlockManager::alloc<16>()
{
    return poolAlloc(m_pool16k, 16);
}

}}

