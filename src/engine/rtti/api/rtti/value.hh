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
    raw<const RTTI::ClassInfo> const    m_type;
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
    enum AsRefType { AsRef };
    template< typename T > explicit inline Value(const T& t);
    template< typename T > explicit inline Value(const T& t, AsRefType asref);
    inline Value(const Value& other);
    inline Value(Value& other, AsRefType asref);
    inline ~Value();

    inline raw<const RTTI::ClassInfo> type() const;

    template< typename T > inline const T& as() const;
    template< typename T > inline T& as();

    inline Value& operator=(const Value& other);
};

}

#include   <rtti/value.inl>

/*****************************************************************************/
#endif
