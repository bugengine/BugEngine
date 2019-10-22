#if 0
namespace minitl
{

template< typename T >
void swap(T& t1, T& t2)
{
    T t = t1;
    t1 = t2;
    t2 = t;
}

class Allocator
{
public:
    template< typename T >
    class Block
    {
    private:
        Allocator*  m_allocator;
        u64         m_count;
        T*          m_data;
    public:
        Block(Allocator& allocator, u64 count, u64 blockAlignment = 4)
            :   m_allocator(&allocator)
            ,   m_count(count)
            ,   m_data(count
                       ? (T*)allocator.alloc(align(sizeof(T), sizeof(T))*count,
                                             max(blockAlignment, sizeof(T)))
                       : 0)
        {
        };
        ~Block()
        {
            m_allocator->free(m_data);
        }
        inline Allocator& arena() const         { return *m_allocator; }
        T* data()                               { return m_data; }
        const T* data() const                   { return m_data; }
        operator T*()                           { return m_data; }
        operator const T*() const               { return m_data; }
        u64 count() const                       { return m_count; }
        u64 byteCount() const                   { return align(sizeof(T), sizeof(T))*m_count; }
        T* begin()                              { return m_data; }
        T* end()                                { return m_data + m_count; }
        const T* begin() const                  { return m_data; }
        const T* end() const                    { return m_data + m_count; }

        bool resize(u64 count)
        {
            u64 size = align(sizeof(T), sizeof(T))*count;
            if (m_allocator->resize(m_data, size))
            {
                m_count = count;
                return true;
            }
            else
            {
                return false;
            }
        }
        void realloc(u64 count, u64 blockAlignment = 4)
        {
            if (count > m_count)
            {
                u64 alignment = max(blockAlignment, sizeof(T));
                u64 size = align(sizeof(T), sizeof(T))*count;
                m_count = count;
                m_data = (T*)m_allocator->realloc(m_data, size, alignment);
            }
            else
            {
                m_count = count;
                if (!count)
                {
                    m_allocator->free(m_data);
                    m_data = 0;
                }
            }
        }
        void swap(Block<T>& other)
        {
            minitl::swap(m_allocator, other.m_allocator);
            minitl::swap(m_count, other.m_count);
            minitl::swap(m_data, other.m_data);
        }
    };
protected:
    virtual void* internalAlloc(u64 size, u64 alignment) = 0;
    virtual bool  internalResize(void* ptr, u64 size) = 0;
    virtual void* internalRealloc(void* ptr, u64 size, u64 alignment) = 0;
    virtual void  internalFree(const void* pointer) = 0;
    virtual ~Allocator() { }
public:
    inline void* alloc(u64 size, u64 alignment = 4);
    inline bool  resize(void* ptr, u64 size);
    inline void* realloc(void* ptr, u64 size, u64 alignment);
    inline void  free(const void* pointer);
    inline char* strdup(const char* src);
    inline char* strdup(const char* begin, const char* end);
    template< typename T >
    inline T* alloc();
};

void* Allocator::alloc(u64 size, u64 alignment)
{
    return internalAlloc(size, alignment);
}

bool  Allocator::resize(void* ptr, u64 size)
{
    return internalResize(ptr, size);
}

void* Allocator::realloc(void* ptr, u64 size, u64 alignment)
{
    return internalRealloc(ptr, size, alignment);
}

void  Allocator::free(const void* pointer)
{
    internalFree(pointer);
}

char* Allocator::strdup(const char* src)
{
    size_t s = strlen(src);
    char *result = static_cast<char*>(internalAlloc(s+1, 1));
    strcpy(result, src);
    return result;
}

char* Allocator::strdup(const char* begin, const char* end)
{
    size_t s = end - begin;
    char *result = static_cast<char*>(internalAlloc(s+1, 1));
    strncpy(result, begin, s);
    result[s] = '\0';
    return result;
}

template< typename T >
T* Allocator::alloc()
{
    return (T*)alloc(sizeof(T), sizeof(T));
}

typedef size_t      size_type;
typedef ptrdiff_t   difference_type;

struct input_iterator_tag                                               { };
struct output_iterator_tag                                              { };
struct forward_iterator_tag : public input_iterator_tag                 { };
struct bidirectional_iterator_tag : public forward_iterator_tag         { };
struct random_access_iterator_tag : public bidirectional_iterator_tag   { };

template< typename T, typename DIFF = ::minitl::difference_type >
struct input_iterator
{
};

template< typename T, typename DIFF = ::minitl::difference_type >
struct output_iterator
{
};

template< typename T, typename DIFF = ::minitl::difference_type >
struct forward_iterator : public input_iterator<T,DIFF>
{
};

template< typename T, typename DIFF = ::minitl::difference_type >
struct bidirectional_iterator : public forward_iterator<T,DIFF>
{
};

template< typename T, typename DIFF = ::minitl::difference_type >
struct random_access_iterator : public bidirectional_iterator<T,DIFF>
{
};



template< typename ITERATOR >
struct iterator_traits
{
    typedef typename ITERATOR::iterator_category    iterator_category;
    typedef typename ITERATOR::value_type           value_type;
    typedef typename ITERATOR::pointer              pointer;
    typedef typename ITERATOR::reference            reference;
    typedef typename ITERATOR::size_type            size_type;
    typedef typename ITERATOR::difference_type      difference_type;
};
template< typename T >
struct iterator_traits<T*>
{
    typedef random_access_iterator_tag  iterator_category;
    typedef T                           value_type;
    typedef T*                          pointer;
    typedef T&                          reference;
    typedef minitl::size_type           size_type;
    typedef minitl::difference_type     difference_type;
};
template< typename T >
struct iterator_traits<const T*>
{
    typedef random_access_iterator_tag  iterator_category;
    typedef const T                     value_type;
    typedef const T*                    pointer;
    typedef const T&                    reference;
    typedef minitl::size_type           size_type;
    typedef minitl::difference_type     difference_type;
};

template< typename ITERATOR >
static typename iterator_traits<ITERATOR>::iterator_category iterator_category(const ITERATOR& it);

template< typename ITERATOR >
static typename iterator_traits<ITERATOR>::difference_type distance(const ITERATOR& first, const ITERATOR& last);

template< typename ITERATOR >
static ITERATOR advance(const ITERATOR& it, typename iterator_traits<ITERATOR>::difference_type offset);

}

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
private:
    void ensure(size_type size);
public:
    explicit vector(Allocator& allocator, size_type initialCapacity = 0);
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


namespace minitl
{

template< typename T >
template< typename POLICY >
class vector<T>::base_iterator
    :   public random_access_iterator<T, typename vector<T>::difference_type>
{
    friend class vector<T>;
public:
    typedef          random_access_iterator_tag         iterator_category;
    typedef typename POLICY::value_type                 value_type;
    typedef typename POLICY::pointer                    pointer;
    typedef typename POLICY::reference                  reference;
    typedef typename POLICY::size_type                  size_type;
    typedef typename POLICY::difference_type            difference_type;
public:
    const vector<T>*            m_owner;
    typename POLICY::pointer    m_iterator;
private:
    base_iterator(const vector<T>* owner, typename POLICY::pointer it);
public:
    base_iterator();
    template< typename OTHERPOLICY >
    base_iterator(const base_iterator<OTHERPOLICY>& other);
    ~base_iterator();
public:
    bool operator==(const base_iterator<POLICY>& other);
    bool operator!=(const base_iterator<POLICY>& other);

    base_iterator<POLICY>& operator=(const base_iterator<POLICY>& other)
    {
        m_owner = other.m_owner;
        m_iterator = other.m_iterator;
        return *this;
    }
    base_iterator<POLICY> operator+(typename POLICY::difference_type offset) const
    {
        return base_iterator<POLICY>(m_owner, POLICY::advance(m_iterator, offset));
    }
    base_iterator<POLICY> operator-(typename POLICY::difference_type offset) const
    {
        return base_iterator<POLICY>(m_owner, POLICY::advance(m_iterator, -offset));
    }
    typename POLICY::difference_type operator-(const base_iterator<POLICY>& other) const
    {
        be_assert_recover(m_owner == other.m_owner, "can't differ between unrelated iterators", return 0);
        return POLICY::distance(other.m_iterator, m_iterator);
    }

    base_iterator<POLICY>& operator++()
    {
        m_iterator = POLICY::advance(m_iterator, 1);
        return *this;
    }
    base_iterator<POLICY>  operator++(int)
    {
        base_iterator<POLICY> p = *this;
        m_iterator = POLICY::advance(m_iterator, 1);
        return p;
    }
    base_iterator<POLICY>& operator+=(typename POLICY::difference_type size)
    {
        m_iterator = POLICY::advance(m_iterator, size);
        return *this;
    }
    base_iterator<POLICY>& operator--()
    {
        m_iterator = POLICY::advance(m_iterator, -1);
        return *this;
    }
    base_iterator<POLICY>  operator--(int)
    {
        base_iterator<POLICY> p = *this;
        m_iterator = POLICY::advance(m_iterator, -1);
        return p;
    }
    base_iterator<POLICY>& operator-=(typename POLICY::difference_type size)
    {
        m_iterator = POLICY::advance(m_iterator, -size);
        return *this;
    }
    typename POLICY::pointer    operator->() const;
    typename POLICY::reference  operator*() const;
};

template< typename T >
template< typename POLICY >
vector<T>::base_iterator<POLICY>::base_iterator()
:   m_owner(0)
,   m_iterator(0)
{
}

template< typename T >
template< typename POLICY >
vector<T>::base_iterator<POLICY>::base_iterator(const vector<T>* owner, typename POLICY::pointer it)
:   m_owner(owner)
,   m_iterator(it)
{
}

template< typename T >
template< typename POLICY >
template< typename OTHERPOLICY >
vector<T>::base_iterator<POLICY>::base_iterator(const base_iterator<OTHERPOLICY>& other)
:   m_owner(other.m_owner)
,   m_iterator(other.m_iterator)
{
}

template< typename T >
template< typename POLICY >
vector<T>::base_iterator<POLICY>::~base_iterator()
{
}

template< typename T >
template< typename POLICY >
bool vector<T>::base_iterator<POLICY>::operator==(const base_iterator<POLICY>& other)
{
    return m_iterator == other.m_iterator;
}

template< typename T >
template< typename POLICY >
bool vector<T>::base_iterator<POLICY>::operator!=(const base_iterator<POLICY>& other)
{
    return m_iterator != other.m_iterator;
}

template< typename T >
template< typename POLICY >
typename POLICY::pointer vector<T>::base_iterator<POLICY>::operator->() const
{
    return m_iterator;
}

template< typename T >
template< typename POLICY >
typename POLICY::reference vector<T>::base_iterator<POLICY>::operator*() const
{
    return *m_iterator;
}


template< typename T >
struct vector<T>::iterator_policy
{
    typedef typename vector<T>::value_type       value_type;
    typedef typename vector<T>::pointer          pointer;
    typedef typename vector<T>::reference        reference;
    typedef typename vector<T>::size_type        size_type;
    typedef typename vector<T>::difference_type  difference_type;
    static pointer advance(pointer i, ptrdiff_t offset) { return minitl::advance(i, offset); }
    static difference_type distance(pointer begin,pointer end) { return minitl::distance(begin, end); }
};

template< typename T >
struct vector<T>::const_iterator_policy
{
    typedef typename vector<T>::value_type const value_type;
    typedef typename vector<T>::const_pointer    pointer;
    typedef typename vector<T>::const_reference  reference;
    typedef typename vector<T>::size_type        size_type;
    typedef typename vector<T>::difference_type  difference_type;
    static pointer advance(pointer i, ptrdiff_t offset) { return minitl::advance(i, offset); }
    static difference_type distance(pointer begin,pointer end) { return minitl::distance(begin, end); }
};

template< typename T >
struct vector<T>::reverse_iterator_policy
{
    typedef typename vector<T>::value_type       value_type;
    typedef typename vector<T>::pointer          pointer;
    typedef typename vector<T>::reference        reference;
    typedef typename vector<T>::size_type        size_type;
    typedef typename vector<T>::difference_type  difference_type;
    static pointer advance(pointer i, ptrdiff_t offset) { return minitl::advance(i, -offset); }
    static difference_type distance(pointer begin,pointer end) { return minitl::distance(end, begin); }
};

template< typename T >
struct vector<T>::const_reverse_iterator_policy
{
    typedef typename vector<T>::value_type const value_type;
    typedef typename vector<T>::const_pointer    pointer;
    typedef typename vector<T>::const_reference  reference;
    typedef typename vector<T>::size_type        size_type;
    typedef typename vector<T>::difference_type  difference_type;
    static pointer advance(pointer i, ptrdiff_t offset) { return minitl::advance(i, -offset); }
    static difference_type distance(pointer begin,pointer end) { return minitl::distance(end, begin); }
};


template< typename T >
vector<T>::vector(Allocator& allocator, size_type initialCapacity)
:   m_memory(allocator, initialCapacity)
,   m_end(m_memory)
{
}

template< typename T >
vector<T>::vector(const vector& other)
    :   m_memory(other.m_memory.arena(), other.size())
    ,   m_end(m_memory)
{
    push_back(other.begin(), other.end());
}

template< typename T >
template< typename ITERATOR >
vector<T>::vector(Allocator& allocator, ITERATOR first, ITERATOR last)
    :   m_memory(allocator, minitl::distance(first, last))
    ,   m_end(m_memory)
{
    push_back(first, last);
}

template< typename T >
vector<T>& vector<T>::operator=(const vector<T>& other)
{
    clear();
    push_back(other.begin(), other.end());
    return *this;
}

template< typename T >
vector<T>::~vector()
{
    for (const_pointer t = m_end; t > m_memory; t = advance(t, -1))
    {
        advance(t, -1)->~T();
    }
}

template< typename T >
typename vector<T>::iterator                 vector<T>::begin()
{
    return iterator(this, m_memory);
}

template< typename T >
typename vector<T>::iterator                 vector<T>::end()
{
    return iterator(this, m_end);
}

template< typename T >
typename vector<T>::const_iterator           vector<T>::begin() const
{
    return const_iterator(this, m_memory);
}

template< typename T >
typename vector<T>::const_iterator           vector<T>::end() const
{
    return const_iterator(this, m_end);
}

template< typename T >
typename vector<T>::reverse_iterator         vector<T>::rbegin()
{
    return reverse_iterator(this, advance(m_end, -1));
}

template< typename T >
typename vector<T>::reverse_iterator         vector<T>::rend()
{
    return reverse_iterator(this, advance(m_memory.data(), -1));
}

template< typename T >
typename vector<T>::const_reverse_iterator   vector<T>::rbegin() const
{
    return const_reverse_iterator(this, advance(m_end, -1));
}

template< typename T >
typename vector<T>::const_reverse_iterator   vector<T>::rend() const
{
    return const_reverse_iterator(this, advance(m_memory.data(), -1));
}


template< typename T >
typename vector<T>::size_type                vector<T>::size() const
{
    return distance(m_memory.data(), m_end);
}

template< typename T >
bool                                         vector<T>::empty() const
{
    return m_end == m_memory;
}


template< typename T >
typename vector<T>::reference                vector<T>::operator[](size_type i)
{
    return *advance(m_memory.data(), i);
}

template< typename T >
typename vector<T>::const_reference          vector<T>::operator[](size_type i) const
{
    return *advance(m_memory.data(), i);
}


template< typename T >
void                                         vector<T>::push_back(const_reference r)
{
    ensure(size() + 1);
    new((void*)m_end) T(r);
    m_end = advance_ptr(m_end, 1);
}

template< typename T >
template< typename ITERATOR >
void                                         vector<T>::push_back(ITERATOR first, ITERATOR last)
{
    size_type count = minitl::distance(first, last);
    ensure(size() + count);
    while (first != last)
    {
        new((void*)m_end) T(*first);
        m_end = advance_ptr(m_end, 1);
        ++first;
    }
}

template< typename T >
void                                         vector<T>::pop_back()
{
    m_end = advance_ptr(m_end, -1);
    m_end->~T();
}

template< typename T >
typename vector<T>::iterator                 vector<T>::erase(iterator it)
{
    return erase(it, it+1);
}

template< typename T >
typename vector<T>::iterator                 vector<T>::erase(iterator first, iterator last)
{
    be_assert_recover(first.m_owner == this, "can't erase iterator that is not pointing on current vector", return first);
    be_assert_recover(last.m_owner == this, "can't erase iterator that is not pointing on current vector", return first);
    be_assert_recover(m_memory <= first.m_iterator && m_end > first.m_iterator, "first %p is not in the range of the vector [%p,%p)" | first.m_iterator | m_memory.data() | m_end, return first);
    be_assert_recover(m_memory <= last.m_iterator && m_end >= last.m_iterator, "last %p is not in the range of the vector [%p,%p)" | last.m_iterator | m_memory.data() | m_end, return first);
    be_assert_recover(first.m_iterator <= last.m_iterator,"first %p is not before last %p" | first.m_iterator | last.m_iterator, return first);

    for (pointer i = first.m_iterator; i != last.m_iterator; i = advance_ptr(i, 1))
    {
        i->~T();
    }
    pointer t = first.m_iterator;
    pointer t2 = last.m_iterator;
    for ( ; t2 != m_end; t = advance(t, 1), t2 = advance_ptr(t2, 1))
    {
        new((void*)t) T(*t2);
        t2->~T();
    }
    m_end = t;
    return first;
}


template< typename T >
typename vector<T>::reference                vector<T>::front()
{
    be_assert(!empty(), "getting front of empty vector");
    return *m_memory;
}

template< typename T >
typename vector<T>::reference                vector<T>::back()
{
    be_assert(!empty(), "getting back of empty vector");
    return *advance_ptr(m_end, -1);
}

template< typename T >
typename vector<T>::const_reference          vector<T>::front() const
{
    be_assert(!empty(), "getting front of empty vector");
    return *m_memory;
}

template< typename T >
typename vector<T>::const_reference          vector<T>::back() const
{
    be_assert(!empty(), "getting front of empty vector");
    return *m_memory;
}


template< typename T >
void                                         vector<T>::resize(size_type size)
{
    size_type s = distance(m_memory.data(), m_end);
    if (size > s)
    {
        ensure(size);
        pointer newend = advance_ptr(m_memory.data(), size);
        for (pointer t = m_end; t != newend; ++t)
            new((void*)t) T;
        m_end = newend;
    }
    else
    {
        pointer newend = advance_ptr(m_memory.data(), size);
        for (pointer t = newend; t != m_end; ++t)
            t->~T();
        m_end = newend;
    }
}

template< typename T >
void                                         vector<T>::clear()
{
    for (pointer t = m_memory.data(); t != m_end; ++t)
        t->~T();
    m_end = m_memory.data();
}

template< typename T >
void                                         vector<T>::ensure(size_type size)
{
    if (size > m_memory.count())
    {
        size = size >> 1  | size;
        size = size >> 2  | size;
        size = size >> 4  | size;
        size = size >> 8  | size;
        size = size >> 16 | size;
        size = size >> (sizeof(size_type) == 64 ? 32 : 0) | size;
        size++;
        reserve(size);
    }
}

template< typename T >
void                                         vector<T>::reserve(size_type size)
{
    if (size > m_memory.count())
    {
        Allocator::Block<T> block(m_memory.arena(), size);
        pointer t = block;
        for (pointer t2 = m_memory; t2 != m_end; t = advance_ptr(t, 1), t2 = advance_ptr(t2, 1))
        {
            new((void*)t) T(*t2);
            t2->~T();
        }
        m_memory.swap(block);
        m_end = t;
    }
}

template< typename T >
void swap(vector<T>& t1, vector<T>& t2)
{
    t1.m_memory.swap(t2.m_memory);
    swap(t1.m_end, t2.m_end);
}

}

void kmain(u32 index, u32 total)
{

}
#endif