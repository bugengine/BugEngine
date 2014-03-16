/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_MINITL_BITSET_HH_
#define BE_MINITL_BITSET_HH_
/**************************************************************************************************/
#include    <minitl/stdafx.h>

namespace minitl
{

template< u32 BITS >
struct bitset
{
private:
    enum
    {
        Bits = BITS,
        Bytes = (BITS+7)/8,
        Words = (BITS+31)/32
    };
    u8 m_bytes[Bytes];
    struct bit
    {
        bitset<BITS>&   owner;
        u32 const       index;
        inline operator bool() const;
        inline bit& operator=(bool);

        bit(bitset<BITS>& owner, u32 index)
            :   owner(owner)
            ,   index(index)
        {
        }
    };
    friend struct bit;
public:
    inline bitset();

    inline bit          operator[](u32 index);
    inline bool         operator[](u32 index) const;

    inline bitset<BITS>& operator &=(const bitset<BITS>& rhs);
    inline bitset<BITS>& operator |=(const bitset<BITS>& rhs);
    inline bitset<BITS>& operator ^=(const bitset<BITS>& rhs);
    inline bitset<BITS>  operator ~() const;

    u32 operator()(u32 begin, u32 end) const;

    inline bool operator ==(const bitset<BITS>& rhs) const;
    inline bool operator !=(const bitset<BITS>& rhs) const;
};

template< u32 BITS >
inline bitset<BITS> operator&(const bitset<BITS>& lhs, const bitset<BITS>& rhs)
{
    bitset<BITS> result = lhs;
    result &= rhs;
    return result;
}

template< u32 BITS >
inline bitset<BITS> operator|(const bitset<BITS>& lhs, const bitset<BITS>& rhs)
{
    bitset<BITS> result = lhs;
    result |= rhs;
    return result;
}

template< u32 BITS >
inline bitset<BITS> operator^(const bitset<BITS>& lhs, const bitset<BITS>& rhs)
{
    bitset<BITS> result = lhs;
    result ^= rhs;
    return result;
}

}

#include <minitl/inl/bitset.inl>

/**************************************************************************************************/
#endif
