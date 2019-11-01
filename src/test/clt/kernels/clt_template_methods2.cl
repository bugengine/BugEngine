template< typename T >
struct vector
{
    template< typename POLICY >
    struct base_iterator
    {
        base_iterator();
        template< typename OTHERPOLICY >
        base_iterator(const base_iterator<OTHERPOLICY>& other);
    };
};


template< typename T >
template< typename POLICY >
template< typename OTHERPOLICY >
vector<T>::base_iterator<POLICY>::base_iterator(const base_iterator<OTHERPOLICY>& other)
:   m_owner(other.m_owner)
,   m_iterator(other.m_iterator)
{
}

void kmain(u32 index, u32 total)
{
    
}