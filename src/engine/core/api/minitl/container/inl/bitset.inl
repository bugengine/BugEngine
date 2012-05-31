/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_CONTAINER_INL_BITSET_INL_
#define BE_MINITL_CONTAINER_INL_BITSET_INL_
/*****************************************************************************/

namespace minitl
{

template< u32 BITS >
bitset<BITS>::bitset()
{
    for (u32 i = 0; i < (u32)Words; ++i)
    {
        m_bits[i] = 0;
    }
}

template< u32 BITS >
typename bitset<BITS>::bit const bitset<BITS>::operator[](u32 index)
{
    be_assert(index < BITS, "bit index %d out of range (size: %d)"|index|BITS);
    bit result = { *this, index };
    return result;
}

template< u32 BITS >
bool bitset<BITS>::operator[](u32 index) const
{
    be_assert(index < BITS, "bit index %d out of range (size: %d)"|index|BITS);
    return 1 & (m_bits[index/32] >> (index%32));
}

template< u32 BITS >
bitset<BITS>& bitset<BITS>::operator &=(const bitset<BITS>& rhs)
{
    for (u32 i = 0; i < (u32)Words; ++i)
    {
        m_bits[i] &= rhs.m_bits[i];
    }
    return *this;
}

template< u32 BITS >
bitset<BITS>& bitset<BITS>::operator |=(const bitset<BITS>& rhs)
{
    for (u32 i = 0; i < (u32)Words; ++i)
    {
        m_bits[i] |= rhs.m_bits[i];
    }
    return *this;
}

template< u32 BITS >
bitset<BITS>& bitset<BITS>::operator ^=(const bitset<BITS>& rhs)
{
    for (u32 i = 0; i < (u32)Words; ++i)
    {
        m_bits[i] ^= rhs.m_bits[i];
    }
    return *this;
}

template< u32 BITS >
bitset<BITS> bitset<BITS>::operator ~() const
{
    bitset<BITS> result;
    for (u32 i = 0; i < (u32)Words; ++i)
    {
        result.m_bits[i] = ~m_bits[i];
    }
    return result;
}

template< u32 BITS >
bool bitset<BITS>::operator ==(const bitset<BITS>& rhs) const
{
    bool result = true;
    for (u32 i = 0; i < (u32)Words-1; ++i)
    {
        result &= (m_bits[i] == rhs.m_bits[i]);
    }
    result &= ((m_bits[Words-1] & ((1<<BITS%32) - 1)) == (rhs.m_bits[Words-1] & ((1<<BITS%32) - 1)));
    return result;
}

template< u32 BITS >
bool bitset<BITS>::operator !=(const bitset<BITS>& rhs) const
{
    bool result = true;
    for (u32 i = 0; i < (u32)Words-1; ++i)
    {
        result &= (m_bits[i] != rhs.m_bits[i]);
    }
    result &= ((m_bits[Words-1] & ((1<<BITS%32) - 1)) != (rhs.m_bits[Words-1]) & ((1<<BITS%32) - 1));
    return result;
}

template< u32 BITS >
bitset<BITS>::bit::operator  bool() const
{
    return 1 & (owner.m_bits[index/32] >> (index%32));
}

template< u32 BITS >
typename bitset<BITS>::bit& bitset<BITS>::bit::operator=(bool value) const
{
    if (value)
    {
        owner.m_bits[index/32] |= (1u << (index%32));
    }
    else
    {
        owner.m_bits[index/32] &= ~(1u << (index%32));
    }
    return *this;
}

}

/*****************************************************************************/
#endif
