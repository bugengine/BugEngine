namespace UnitTest_TemplateMethods3
{
    
template< typename T >
struct vector
{
    template< typename POLICY >
    struct base_iterator;
    typedef base_iterator<u32> iterator;

    iterator begin();
};

template< typename U >
typename vector<U>::iterator vector<U>::begin()
{
}

void kmain(u32 index, u32 total)
{
    vector<i32> v;
}

}