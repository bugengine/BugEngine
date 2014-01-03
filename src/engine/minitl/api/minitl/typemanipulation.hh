/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_MINITL_TYPEMANIPULATION_HH_
#define BE_MINITL_TYPEMANIPULATION_HH_
/*****************************************************************************/


namespace minitl
{

template< typename T > struct is_const              { enum { Value = 0 }; };
template< typename T > struct is_const<const T>     { enum { Value = 1 }; };
template< typename T > struct remove_const          { typedef T type; };
template< typename T > struct remove_const<const T> { typedef T type; };

template< typename T > struct is_reference          { enum { Value = 0 }; };
template< typename T > struct is_reference<T&>      { enum { Value = 1 }; };
template< typename T > struct remove_reference      { typedef T type; };
template< typename T > struct remove_reference<T&>  { typedef T type; };

template< typename T1, typename T2 >
struct type_equals
{
    enum { Value = 0 };
};

template< typename T >
struct type_equals<T, T>
{
    enum { Value = 1 };
};

}

/*****************************************************************************/
#endif
