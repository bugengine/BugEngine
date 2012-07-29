/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_MINITL_FORMAT_H_
#define BE_MINITL_FORMAT_H_
/*****************************************************************************/
#include    <minitl/allocator.hh>

namespace minitl
{

template< u16 SIZE >
class format
{
private:
    mutable char  m_buffer[SIZE];
    mutable char* m_firstFormat;
private:
    void findToken() const;
public:
    format(const char *formatstr);
    ~format();

    inline operator const char* () const
    {
        return m_buffer;
    }
    inline const char* c_str() const
    {
        return m_buffer;
    }

    inline const char *token() const
    {
        return m_firstFormat;
    }
    void put(const char *value) const;
};

template< u16 SIZE >
format<SIZE>::format(const char *formatstr)
    :   m_firstFormat(m_buffer)
{
    strncpy(m_buffer, formatstr, SIZE-1);
    m_buffer[SIZE-1] = 0;
    findToken();
}


template< u16 SIZE >
format<SIZE>::~format()
{
}

template< u16 SIZE >
void format<SIZE>::findToken() const
{
    while (*m_firstFormat && (m_firstFormat[1] == '%' || m_firstFormat[0] != '%'))
    {
        m_firstFormat++;
    }
    if (!m_firstFormat[0])
        m_firstFormat = 0;
}

template< u16 SIZE >
void format<SIZE>::put(const char *value) const
{
    if (!value)
        value = "(null)";
    size_t s = strlen(value);
    if (m_firstFormat+s-m_buffer < SIZE)
    {
        memmove(m_firstFormat+s, m_firstFormat+2, SIZE-(m_firstFormat+s-m_buffer));
        strncpy(m_firstFormat, value, s);
    }
    else
    {
        memmove(m_firstFormat+s, m_firstFormat+2, SIZE-(m_firstFormat-m_buffer));
        strncpy(m_firstFormat, value, SIZE-(m_firstFormat-m_buffer));
    }
    m_firstFormat += s;
    findToken();
}


template< u16 SIZE >
const format<SIZE>& operator|(const format<SIZE>& f, char value)
{
    char str[2] = {value, 0};
    f.put(str);
    return f;
}

template< u16 SIZE >
const format<SIZE>& operator|(const format<SIZE>& f, char *value)
{
    f.put(value);
    return f;
}

template< u16 SIZE >
const format<SIZE>& operator|(const format<SIZE>& f, const char* value)
{
    f.put(value);
    return f;
}

template< u16 SIZE >
const format<SIZE>& operator|(const format<SIZE>& f, const format<SIZE>& value)
{
    f.put(value.c_str());
    return f;
}

template< u16 SIZE >
const format<SIZE>& operator|(const format<SIZE>& f, i64 value)
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


template< u16 SIZE >
const format<SIZE>& operator|(const format<SIZE>& f, u64 value)
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

template< u16 SIZE >
const format<SIZE>& operator|(const format<SIZE>& f, i32 value)
{
    return f | (i64)value;
}

template< u16 SIZE >
const format<SIZE>& operator|(const format<SIZE>& f, i16 value)
{
    return f | (i64)value;
}

template< u16 SIZE, typename T >
const format<SIZE>& operator|(const format<SIZE>& f, T value)
{
    return f | static_cast<u64>(value);
}

template< u16 SIZE, typename T >
const format<SIZE>& operator|(const format<SIZE>& f, const T* value)
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

template< u16 SIZE, typename T >
const format<SIZE>& operator|(const format<SIZE>& f, T* value)
{
    return f | (const T*)value;
}

}

/*****************************************************************************/
#endif
