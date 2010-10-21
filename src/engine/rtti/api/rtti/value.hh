/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_VALUE_HH_
#define BE_RTTI_VALUE_HH_
/*****************************************************************************/
#include   <rtti/typeinfo.hh>

namespace BugEngine
{

class Value
{
private:
    TypeInfo    m_type;
    union
    {
        char                            m_buffer[32];
        struct
        {
            void*                       m_pointer;
            bool                        m_deallocate;
        };
    };
private:
    inline void* memory();
    inline const void* memory() const;
public:
    template< typename T > explicit inline Value(T t);
    inline Value(const Value& other);
    inline ~Value();

    inline TypeInfo type() const;

    template< typename T > inline const T& as() const;
    template< typename T > inline T& as();

    inline Value& operator=(const Value& other);
};

}

#include   <rtti/value.inl>

/*****************************************************************************/
#endif
