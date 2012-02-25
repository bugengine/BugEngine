/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_RUNTIME_MD5_HH_
#define BE_CORE_RUNTIME_MD5_HH_
/*****************************************************************************/
#include    <minitl/string/format.hh>

namespace BugEngine
{

struct MD5
{
    u32 hash[4];
};

static inline bool operator==(const MD5& hash1, const MD5& hash2)
{
    return hash1.hash[0] == hash2.hash[0]
        && hash1.hash[1] == hash2.hash[1]
        && hash1.hash[2] == hash2.hash[2]
        && hash1.hash[3] == hash2.hash[3];
}

be_api(CORE) MD5 digest(const void* data, u64 size);

template< typename T >
static inline MD5 digest(const Allocator::Block<T>& block)
{
    return digest(block.data(), block.byteCount());
}

}

namespace minitl
{

static char hex(unsigned char value)
{
    return value >= 10 ? value-10+'a' : value + '0';
}

template< u16 size >
const format<size>& operator|(const format<size>& f, const BugEngine::MD5& value)
{
    char str[33];
    for(int i = 0; i < 4; ++i)
    {
        str[i*8+0] = hex((unsigned char)(0xf & (value.hash[i] >> 4)));
        str[i*8+1] = hex((unsigned char)(0xf & (value.hash[i] >> 0)));
        str[i*8+2] = hex((unsigned char)(0xf & (value.hash[i] >> 12)));
        str[i*8+3] = hex((unsigned char)(0xf & (value.hash[i] >> 8)));
        str[i*8+4] = hex((unsigned char)(0xf & (value.hash[i] >> 20)));
        str[i*8+5] = hex((unsigned char)(0xf & (value.hash[i] >> 16)));
        str[i*8+6] = hex((unsigned char)(0xf & (value.hash[i] >> 28)));
        str[i*8+7] = hex((unsigned char)(0xf & (value.hash[i] >> 24)));
    }
    str[32] = 0;
    return f | str;
}

}

/*****************************************************************************/
#endif

