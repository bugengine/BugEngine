/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_BITSET_HH_
#define BE_MINITL_BITSET_HH_
/*****************************************************************************/

namespace minitl
{

template< u32 BITS >
class bitset
{
private:
    enum
    {
        Bits = BITS,
        Bytes = (BITS+7)/8,
        Words = (BITS+21)/32
    };
    u32 m_bits[Words];
    struct bit
    {
        bitset<BITS>    owner;
        u32 const       index;
        inline operator bool() const;
        inline bit& operator=(bool) const;
    };
    friend struct bit;
public:
    inline bitset();

    inline const bit    operator[](u32 index);
    inline bool         operator[](u32 index) const;

    inline bitset<BITS>& operator &=(const bitset<BITS>& rhs);
    inline bitset<BITS>& operator |=(const bitset<BITS>& rhs);
    inline bitset<BITS>& operator ^=(const bitset<BITS>& rhs);
    inline bitset<BITS>  operator ~() const;

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

/*****************************************************************************/
#endif
