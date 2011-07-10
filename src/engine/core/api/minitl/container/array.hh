/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_CONTAINER_ARRAY_HH_
#define BE_MINITL_CONTAINER_ARRAY_HH_
/*****************************************************************************/
#include    <minitl/container/iterator.hh>
#include    <core/memory/malloc.hh>

namespace minitl
{

template< typename T, size_t SIZE >
class array
{
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
    u8  m_memory[SIZE*sizeof(T)];
    T*  m_end;
public:
                                        inline explicit array(size_type count = 0);
                                        inline array(const array<T, SIZE>& other);
    template< typename T1, size_t S1 >  inline array(const array<T1,S1>& other);
    template< typename IT >             inline array(IT first, IT last);
                                        inline ~array();

                                        inline array&                  operator=(const array<T, SIZE>& other);
    template< typename T1, size_t S1 >  inline array&                  operator=(const array<T1, S1>& other);
                                        inline iterator                begin();
                                        inline iterator                end();
                                        inline const_iterator          begin() const;
                                        inline const_iterator          end() const;
                                        inline reverse_iterator        rbegin();
                                        inline reverse_iterator        rend();
                                        inline const_reverse_iterator  rbegin() const;
                                        inline const_reverse_iterator  rend() const;

                                        inline size_type               size() const;
                                        inline bool                    empty() const;

                                        inline reference               operator[](size_type i);
                                        inline const_reference         operator[](size_type i) const;

                                        inline void                    push_back(const_reference r);
    template< typename IT >             inline void                    push_back(IT first, IT last);
                                        inline iterator                erase(iterator it);
                                        inline iterator                erase(iterator begin, iterator end);

                                        inline reference               front();
                                        inline reference               back();
                                        inline const_reference         front() const;
                                        inline const_reference         back() const;

                                        inline void                    clear();
};

}

#include <minitl/container/inl/array.inl>

/*****************************************************************************/
#endif
