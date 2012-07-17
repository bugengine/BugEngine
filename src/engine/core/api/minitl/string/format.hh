/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_FORMAT_H_
#define BE_MINITL_FORMAT_H_
/*****************************************************************************/
#include    <core/debug/assert.hh>
#include    <string.h>

namespace minitl
{

template< u16 size = 512 >
class format
{
private:
    mutable char    m_buffer[size];
    mutable char*   m_firstFormat;
private:
    void findToken() const;
public:
    inline format(const char *formatstr);
    inline ~format();

    inline operator const char* () const;
    inline const char* c_str() const;

    inline const char *token() const;
    inline void put(const char *value) const;
    inline void append(const char *value);
};


template< u16 size >
format<size>::format(const char *formatstr)
{
    strncpy(m_buffer, formatstr, size);
    m_buffer[size-1] = 0;
    m_firstFormat = m_buffer;
    findToken();
}

template< u16 size >
void format<size>::findToken() const
{
    while (*m_firstFormat && (m_firstFormat[1] == '%' || m_firstFormat[0] != '%'))
    {
        m_firstFormat++;
    }
    if (!m_firstFormat[0])
        m_firstFormat = 0;
}

template< u16 size >
format<size>::~format()
{
}

template< u16 size >
format<size>::operator const char* () const
{
    return m_buffer;
}

template< u16 size >
const char* format<size>::c_str() const
{
    return m_buffer;
}

template< u16 size >
const char* format<size>::token() const
{
    return m_firstFormat;
}

template< u16 size >
void format<size>::put(const char *value) const
{
    if (!value)
        value = "(null)";
    size_t s = strlen(value);
    memmove(m_firstFormat+s, m_firstFormat+2, size-(m_firstFormat+s-m_buffer));
    strncpy(m_firstFormat, value, s);
    m_firstFormat += s;
    findToken();
}

template< u16 size >
const format<size>& operator|(const format<size>& f, char value)
{
    char str[2] = {value, 0};
    f.put(str);
    return f;
}

template< u16 size >
const format<size>& operator|(const format<size>& f, char *value)
{
    f.put(value);
    return f;
}

template< u16 size >
const format<size>& operator|(const format<size>& f, const char* value)
{
    f.put(value);
    return f;
}

template< u16 size, u16 osize >
const format<size>& operator|(const format<size>& f, const format<osize>& value)
{
    f.put(value.c_str());
    return f;
}

template< u16 size >
const format<size>& operator|(const format<size>& f, i64 value)
{
    char result[16];
    result[15] = 0;
    char* buf = result+14;
    bool sign = (value < 0);
    value = sign? -value : value;
    do
    {
        *(buf--) = (char)(value%10)+'0';
        value /= 10;
    } while (value);
    if (sign)
    {
        *(buf--) = '-';
    }
    f.put(buf+1);
    return f;
}


template< u16 size >
const format<size>& operator|(const format<size>& f, u64 value)
{
    char result[16];
    result[15] = 0;
    char* buf = result+14;
    do
    {
        *(buf--) = (char)(value%10)+'0';
        value /= 10;
    } while (value);
    f.put(buf+1);
    return f;
}

template< u16 size >
const format<size>& operator|(const format<size>& f, i32 value)
{
    return f | (i64)value;
}

template< u16 size >
const format<size>& operator|(const format<size>& f, i16 value)
{
    return f | (i64)value;
}

template< u16 size, typename T >
const format<size>& operator|(const format<size>& f, T value)
{
    return f | static_cast<u64>(value);
}

template< u16 size, typename T >
const format<size>& operator|(const format<size>& f, const T* value)
{
    static const size_t s = 2+sizeof(value)*2;
    char result[s+1];
    result[0] = '0';
    result[1] = 'x';
    result[s] = 0;
    for (size_t i = 0; i < sizeof(value)*2; ++i)
    {
        result[i+2] = (char)(((size_t)value >> ((sizeof(value)*2-i-1)*4)) & 0xf) + '0';
        if (result[i+2] > '9')
            result[i+2] = result[i+2]+'A'-'9'-1;
    }
    f.put(result);
    return f;
}

template< u16 size, typename T >
const format<size>& operator|(const format<size>& f, T* value)
{
    return f | (const T*)value;
}

template< u16 size >
void format<size>::append(const char *value)
{
    size_t s = strlen(m_buffer);
    strncat(m_buffer + s, value, size-s-1);
    if (!m_firstFormat)
    {
        m_firstFormat = m_buffer + s;
        findToken();
    }
}



}

/*****************************************************************************/
#endif
