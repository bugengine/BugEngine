/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_BLOCKMANAGER_HH_
#define BE_WORLD_BLOCKMANAGER_HH_
/*****************************************************************************/


namespace BugEngine { namespace World
{


class BlockManager : public minitl::pointer
{
private:
    minitl::itaggedptr<void>    m_pool256k;
    minitl::itaggedptr<void>    m_pool128k;
    minitl::itaggedptr<void>    m_pool64k;
    minitl::itaggedptr<void>    m_pool32k;
    minitl::itaggedptr<void>    m_pool16k;
    i_u32                       m_usedBlocks;
public:
    BlockManager();
    ~BlockManager();

    template< size_t SIZEKB > void* alloc();
    template< size_t SIZEKB > void  free(void* ptr);
    template< size_t SIZEKB > void* realloc();
};

template< > void* BlockManager::alloc<256>();
template< > void* BlockManager::alloc<128>();
template< > void* BlockManager::alloc<64>();
template< > void* BlockManager::alloc<32>();
template< > void* BlockManager::alloc<16>();

template< > void BlockManager::free<256>(void* ptr);
template< > void BlockManager::free<128>(void* ptr);
template< > void BlockManager::free<64>(void* ptr);
template< > void BlockManager::free<32>(void* ptr);
template< > void BlockManager::free<16>(void* ptr);



template< typename T, size_t SIZEKB >
struct Block : public minitl::pointer
{
private:
    weak<BlockManager>      m_manager;
    minitl::vector< u8* >   m_blocks;
public:
    Block(weak<BlockManager> manager)
        :   m_manager(manager)
        ,   m_blocks(gameArena())
    {
    }
    ~Block()
    {
        for (minitl::vector< u8* >::const_iterator it = m_blocks.begin(); it != m_blocks.end(); ++it)
        {
            m_manager->free<SIZEKB>(*it);
        }
    }

    T& operator[](u32 index)
    {
        u32 block = index >> 16;
        index = index & 0xffff;
        if (block >= m_blocks.size() )
        {
            be_assert(block == m_blocks.size(), "access past the end of a block");
            m_blocks.push_back((u8*)manager->alloc<SIZE>());
        }
        u8* buffer = m_blocks[block];
        size_t offset = index*be_align(sizeof(T), be_alignof(T));
        be_assert_recover(offset < SIZEKB*1024, "accessing at index %d past the end of the buffer"|index, return *reinterpret_cast<T*>(buffer));
        return *reinterpret_cast<T*>(buffer+offset);
    }

    size_t capacity() const
    {
        return m_blocks.size() * be_checked_numcast<u16>(SIZEKB/be_align(sizeof(T), be_alignof(T)));
    }
};


}}


/*****************************************************************************/
#endif
