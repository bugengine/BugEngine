/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_MINITL_VECTOR_HH_
#define BE_MINITL_VECTOR_HH_
/*****************************************************************************/
#include    <minitl/iterator.hh>
#include    <minitl/allocator.hh>

namespace minitl
{

template< typename T >
class vector;

template< typename T >
void swap(vector<T>& t1, vector<T>& t2);

template< typename T >
class vector
{
    template< typename U >
    friend void swap(vector<U>& t1, vector<U>& t2);
private:
    template< typename POLICY >
    class base_iterator;
    struct iterator_policy;
    struct const_iterator_policy;
    struct reverse_iterator_policy;
    struct const_reverse_iterator_policy;
public:
    typedef T                       value_type;
    typedef T*                      pointer;
    typedef T&                      reference;
    typedef const T*                const_pointer;
    typedef const T&                const_reference;
    typedef minitl::size_type       size_type;
    typedef minitl::difference_type difference_type;
public:
    typedef base_iterator<iterator_policy>                  iterator;
    typedef base_iterator<const_iterator_policy>            const_iterator;
    typedef base_iterator<reverse_iterator_policy>          reverse_iterator;
    typedef base_iterator<const_reverse_iterator_policy>    const_reverse_iterator;
private:
    Allocator::Block<T> m_memory;
    T*                  m_end;
    T*                  m_capacity;
private:
    void ensure(size_type size);
public:
    explicit vector(Allocator& allocator, size_type count = 0);
    vector(const vector<T>& other);
    vector& operator=(const vector<T>& other);
    template< typename ITERATOR >
    vector(Allocator& allocator, ITERATOR first, ITERATOR last);
    ~vector();

    iterator                begin();
    iterator                end();
    const_iterator          begin() const;
    const_iterator          end() const;
    reverse_iterator        rbegin();
    reverse_iterator        rend();
    const_reverse_iterator  rbegin() const;
    const_reverse_iterator  rend() const;

    size_type               size() const;
    bool                    empty() const;

    reference               operator[](size_type i);
    const_reference         operator[](size_type i) const;

    void                    push_back(const_reference r);
    template< typename ITERATOR >
    void                    push_back(ITERATOR first, ITERATOR last);
    void                    pop_back();
    iterator                erase(iterator it);
    iterator                erase(iterator begin, iterator end);

    reference               front();
    reference               back();
    const_reference         front() const;
    const_reference         back() const;

    void                    reserve(size_type capacity);
    void                    resize(size_type size);
    void                    clear();
};

}

#include <minitl/inl/vector.inl>

/*****************************************************************************/
#endif
