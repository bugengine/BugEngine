/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_WORLD_BLOCKMANAGER_HH_
#define BE_WORLD_BLOCKMANAGER_HH_
/*****************************************************************************/


namespace BugEngine { namespace World
{

template< typename T, size_t SIZE >
struct Block
{
    template< typename U > Block<U, SIZE>* as() { return reinterpret_cast<Block<U, SIZE>*>(this); }
    T& operator[](size_t index)
    {
        union Item
        {
            T t;
            u8 u;
        };
        u8* buffer = reinterpret_cast<u8*>(this);
        size_t offset = index*be_align(sizeof(T), be_alignof(T));
        be_assert_recover(offset < SIZE, "accessing at index %d past the end of the buffer"|index, return *reinterpret_cast<T*>(buffer));
        return *reinterpret_cast<T*>(buffer+offset);
    }
};

class BlockManager : public minitl::pointer
{
private:
    Block<u8, 262144>*  m_pool256k;
    Block<u8, 131072>*  m_pool128k;
    Block<u8,  65536>*  m_pool64k;
    Block<u8,  32768>*  m_pool32k;
    Block<u8,  16384>*  m_pool16k;
    i_u32               m_usedBlocks;
private:
    Block<u8, 262144>* reserve256k();
    Block<u8, 131072>* reserve128k();
    Block<u8,  65536>* reserve64k();
    Block<u8,  32768>* reserve32k();
    Block<u8,  16384>* reserve16k();
public:
    BlockManager();
    ~BlockManager();
    
    template< typename U > Block<U, 262144>* alloc256k() { return reserve256k()->as<U>(); }
    template< typename U > Block<U, 131072>* alloc128k() { return reserve128k()->as<U>(); }
    template< typename U > Block<U,  65536>* alloc64k()  { return reserve64k()->as<U>(); }
    template< typename U > Block<U,  32768>* alloc32k()  { return reserve32k()->as<U>(); }
    template< typename U > Block<U,  16384>* alloc16k()  { return reserve16k()->as<U>(); }
};

}}


/*****************************************************************************/
#endif
