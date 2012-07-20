/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_TRAITS_HH_
#define BE_MINITL_TRAITS_HH_
/*****************************************************************************/
#include <string.h>

namespace minitl
{

#define get16bits(d) (                               \
            (static_cast<u32>((d)[1]) << 8)          \
        +   (static_cast<u32>((d)[0])) )

static inline size_t str_hash(const char * _data, size_t len)
{
    const u8 *data = reinterpret_cast<const u8 *>(_data);
    size_t hashvalue = len, tmp;
    size_t rem;
    if (len == 0 || data == NULL) return 0;

    rem = len & 3;
    len >>= 2;

    for (;len > 0; len--)
    {
        hashvalue  += get16bits (data);
        tmp         = (get16bits (data+2) << 11) ^ hashvalue;
        hashvalue   = (hashvalue << 16) ^ tmp;
        data       += 2*sizeof (u16);
        hashvalue  += hashvalue >> 11;
    }

    switch (rem) {
        case 3: hashvalue += get16bits (data);
                hashvalue ^= hashvalue << 16;
                hashvalue ^= static_cast<u32>(data[sizeof (u16)]) << 18;
                hashvalue += hashvalue >> 11;
                break;
        case 2: hashvalue += get16bits (data);
                hashvalue ^= hashvalue << 11;
                hashvalue += hashvalue >> 17;
                break;
        case 1: hashvalue += *data;
                hashvalue ^= hashvalue << 10;
                hashvalue += hashvalue >> 1;
                break;
        case 0:
        default:
                break;
    }

    hashvalue ^= hashvalue << 3;
    hashvalue += hashvalue >> 5;
    hashvalue ^= hashvalue << 4;
    hashvalue += hashvalue >> 17;
    hashvalue ^= hashvalue << 25;
    hashvalue += hashvalue >> 6;

    return hashvalue;
}




template< typename T >
struct less;

template< >
struct less<const char*>
{
    bool operator()(const char* str1, const char *str2) const { return ::strcmp(str1, str2) < 0; }
};

}

/*****************************************************************************/
#endif
