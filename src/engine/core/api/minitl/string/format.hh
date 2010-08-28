/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_FORMAT_H_
#define BE_MINITL_FORMAT_H_
/*****************************************************************************/
#include    <core/debug/assert.hh>

namespace minitl
{

template< size_t size = 512 >
class format
{
private:
    mutable char    m_buffer[size];
    mutable char*   m_firstFormat;
private:
    void findToken() const;
public:
    inline format(const char *format);
    inline ~format();

    inline operator const char* () const;
    inline const char* c_str() const;

    const format<size>& operator|(char* value) const;
    const format<size>& operator|(const char* value) const;
    const format<size>& operator|(char value) const;
    const format<size>& operator|(u64 value) const;
    const format<size>& operator|(i64 value) const;
    const format<size>& operator|(i32 value) const;
    const format<size>& operator|(i16 value) const;
    template< typename T >
    const format<size>& operator|(T* value) const;
    template< typename T >
    const format<size>& operator|(const T* value) const;
    template< typename T >
    const format<size>& operator|(T value) const;
};


template< size_t size >
format<size>::format(const char *format)
{
    strncpy(m_buffer, format, size);
    m_buffer[size-1] = 0;
    m_firstFormat = m_buffer;
    findToken();
}

template< size_t size >
void format<size>::findToken() const
{
    while(*m_firstFormat && (m_firstFormat[1] == '%' || m_firstFormat[0] != '%'))
    {
        m_firstFormat++;
    }
    if(!m_firstFormat[0])
        m_firstFormat = 0;
}

template< size_t size >
format<size>::~format()
{
}

template< size_t size >
format<size>::operator const char* () const
{
    return m_buffer;
}

template< size_t size >
const char* format<size>::c_str() const
{
    return m_buffer;
}

template< size_t size >
const format<size>& format<size>::operator|(char value) const
{
    m_firstFormat[0] = value;
    memmove(m_firstFormat+1, m_firstFormat+2, size-(m_firstFormat+2-m_buffer));
    m_firstFormat++;
    findToken();
    return *this;
}

template< size_t size >
const format<size>& format<size>::operator|(char *value) const
{
    return *this | (const char *)value;
}

template< size_t size >
const format<size>& format<size>::operator|(const char* value) const
{
    if(!value)
        value = "(null)";
    size_t s = strlen(value);
    memmove(m_firstFormat+s, m_firstFormat+2, size-(m_firstFormat+s-m_buffer));
    strncpy(m_firstFormat, value, s);
    m_firstFormat += s;
    findToken();
    return *this;
}

template< size_t size >
const format<size>& format<size>::operator|(i64 value) const
{
    char result[16];
    char* buf = result;
    if(value < 0)
    {
        *(buf++) = '-';
    }
    do
    {
        *(buf++) = (char)(value%10)+'0';
        value /= 10;
    } while (value);
    size_t s = buf-result;
    memmove(m_firstFormat+s, m_firstFormat+2, size-(m_firstFormat+s-m_buffer));
    while(buf-- != result)
    {
        *(m_firstFormat++) = *buf;
    }
    findToken();
    return *this;
}


template< size_t size >
const format<size>& format<size>::operator|(u64 value) const
{
    char result[16];
    char* buf = result;
    do
    {
        *(buf++) = (char)(value%10)+'0';
        value /= 10;
    } while (value);
    size_t s = buf-result;
    memmove(m_firstFormat+s, m_firstFormat+2, size-(m_firstFormat+s-m_buffer));
    while(buf-- != result)
    {
        *(m_firstFormat++) = *buf;
    }
    findToken();
    return *this;
}

template< size_t size >
const format<size>& format<size>::operator|(i32 value) const
{
    return *this | (i64)value;
}

template< size_t size >
const format<size>& format<size>::operator|(i16 value) const
{
    return *this | (i64)value;
}

template< size_t size >
template< typename T >
const format<size>& format<size>::operator|(T value) const
{
    return *this | static_cast<u64>(value);
}

template< size_t size >
template< typename T >
const format<size>& format<size>::operator|(T* value) const
{
    static const size_t s = 2+sizeof(value)*2;
    char result[s];
    result[0] = '0';
    result[1] = 'x';
    for(size_t i = 0; i < sizeof(value)*2; ++i)
    {
        result[i+2] = (char)(((size_t)value >> ((sizeof(value)*2-i-1)*4)) & 0xf) + '0';
        if(result[i+2] > '9')
            result[i+2] = result[i+2]+'A'-'9'-1;
    }
    memmove(m_firstFormat+s, m_firstFormat+2, size-(m_firstFormat+s-m_buffer));
    strncpy(m_firstFormat, result, s);
    m_firstFormat += s;
    findToken();
    return *this;
}

template< size_t size >
template< typename T >
const format<size>& format<size>::operator|(const T* value) const
{
    static const size_t s = 2+sizeof(value)*2;
    char result[s];
    result[0] = '0';
    result[1] = 'x';
    for(size_t i = 0; i < sizeof(value)*2; ++i)
    {
        result[i+2] = (char)(((size_t)value >> ((sizeof(value)*2-i-1)*4)) & 0xf) + '0';
        if(result[i+2] > '9')
            result[i+2] = result[i+2]+'A'-'9'-1;
    }
    memmove(m_firstFormat+s, m_firstFormat+2, size-(m_firstFormat+s-m_buffer));
    strncpy(m_firstFormat, result, s);
    m_firstFormat += s;
    findToken();
    return *this;
}

}

/*****************************************************************************/
#endif
