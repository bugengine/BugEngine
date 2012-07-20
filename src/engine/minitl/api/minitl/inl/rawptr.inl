/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_PTR_RAWPTR_INL_
#define BE_MINITL_PTR_RAWPTR_INL_
/*****************************************************************************/

namespace minitl
{

template< typename T >
T* raw<T>::operator->() const
{
    return m_ptr;
}

template< typename T >
raw<T>::operator const void*() const
{
    return m_ptr;
}

template< typename T >
bool raw<T>::operator!() const
{
    return m_ptr == 0;
}

template< typename T >
T& raw<T>::operator*()
{
    return *m_ptr;
}

template< typename T >
raw<T> raw<T>::null()
{
    raw<T> result = { 0 };
    return result;
}

}

/*****************************************************************************/
#endif
