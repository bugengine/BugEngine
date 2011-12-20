/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_RUNTIME_MD5_HH_
#define BE_CORE_RUNTIME_MD5_HH_
/*****************************************************************************/

namespace BugEngine
{

struct MD5
{
    u32 hash[4];

    template< typename T >
    inline void parse(const Allocator::Block<T>& block);
private:
    void parse(const void* data, size_t size);
};

static inline bool operator==(const MD5& hash1, const MD5& hash2)
{
    return hash1.hash[0] == hash2.hash[0]
        && hash1.hash[1] == hash2.hash[1]
        && hash1.hash[2] == hash2.hash[2]
        && hash1.hash[3] == hash2.hash[3];
}

template< typename T >
void MD5::parse(const Allocator::Block<T>& block)
{
    parse(block.data(), block.byteCount());
}
}

/*****************************************************************************/
#endif

