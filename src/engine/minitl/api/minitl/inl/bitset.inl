/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_CONTAINER_INL_BITSET_INL_
#define BE_MINITL_CONTAINER_INL_BITSET_INL_
/*****************************************************************************/

namespace minitl
{

template< u32 BITS >
bitset<BITS>::bitset()
{
    for (u32 i = 0; i < (u32)Bytes; ++i)
    {
        m_bytes[i] = 0;
    }
}

template< u32 BITS >
typename bitset<BITS>::bit bitset<BITS>::operator[](u32 index)
{
    be_assert(index < BITS, "bit index %d out of range (size: %d)"|index|BITS);
    bit result = { *this, index };
    return result;
}

template< u32 BITS >
bool bitset<BITS>::operator[](u32 index) const
{
    be_assert(index < BITS, "bit index %d out of range (size: %d)"|index|BITS);
    return 1 & (m_bytes[index/8] >> (index%8));
}

template< u32 BITS >
bitset<BITS>& bitset<BITS>::operator &=(const bitset<BITS>& rhs)
{
    for (u32 i = 0; i < (u32)Bytes; ++i)
    {
        m_bytes[i] &= rhs.m_bytes[i];
    }
    return *this;
}

template< u32 BITS >
bitset<BITS>& bitset<BITS>::operator |=(const bitset<BITS>& rhs)
{
    for (u32 i = 0; i < (u32)Bytes; ++i)
    {
        m_bytes[i] |= rhs.m_bytes[i];
    }
    return *this;
}

template< u32 BITS >
bitset<BITS>& bitset<BITS>::operator ^=(const bitset<BITS>& rhs)
{
    for (u32 i = 0; i < (u32)Bytes; ++i)
    {
        m_bytes[i] ^= rhs.m_bytes[i];
    }
    return *this;
}

template< u32 BITS >
bitset<BITS> bitset<BITS>::operator ~() const
{
    bitset<BITS> result;
    for (u32 i = 0; i < (u32)Bytes; ++i)
    {
        result.m_bytes[i] = ~m_bytes[i];
    }
    return result;
}

template< u32 BITS >
bool bitset<BITS>::operator ==(const bitset<BITS>& rhs) const
{
    bool result = true;
    for (u32 i = 0; i < (u32)Bytes-1; ++i)
    {
        result &= (m_bytes[i] == rhs.m_bytes[i]);
    }
    result &= ((m_bytes[Bytes-1] & ((1<<BITS%8) - 1)) == (rhs.m_bytes[Bytes-1] & ((1<<BITS%8) - 1)));
    return result;
}

template< u32 BITS >
bool bitset<BITS>::operator !=(const bitset<BITS>& rhs) const
{
    bool result = true;
    for (u32 i = 0; i < (u32)Bytes-1; ++i)
    {
        result &= (m_bytes[i] != rhs.m_bytes[i]);
    }
    result &= ((m_bytes[Bytes-1] & ((1<<BITS%8) - 1)) != (rhs.m_bytes[Bytes-1]) & ((1<<BITS%8) - 1));
    return result;
}

template< u32 BITS >
bitset<BITS>::bit::operator  bool() const
{
    return 1 & (owner.m_bytes[index/32] >> (index%32));
}

template< u32 BITS >
typename bitset<BITS>::bit& bitset<BITS>::bit::operator=(bool value)
{
    if (value)
    {
        owner.m_bytes[index/8] |= (1u << (index%8));
    }
    else
    {
        owner.m_bytes[index/8] &= ~(1u << (index%8));
    }
    return *this;
}

}

/*****************************************************************************/
#endif
