/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

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
    char        m_buffer[size];
    char*       m_firstFormat;
private:
    void findToken();
public:
    inline format(const char *format);
    inline ~format();

    inline operator const char* () const;
    inline operator char* ();
    inline const char* c_str() const;

    format<size>& operator|(const char* value);
    format<size>& operator|(const void* value);
    format<size>& operator|(char value);
    format<size>& operator|(u64 value);
    format<size>& operator|(i64 value);
    format<size>& operator|(u32 value);
    format<size>& operator|(i32 value);
    format<size>& operator|(u16 value);
    format<size>& operator|(i16 value);
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
void format<size>::findToken()
{
    Assert(m_firstFormat >= m_buffer && m_firstFormat < m_buffer+size);
    while(*m_firstFormat && (m_firstFormat[1] == '%' || m_firstFormat[0] != '%'))
    {
        m_firstFormat++;
    }
    Assert(m_firstFormat >= m_buffer && m_firstFormat < m_buffer+size);
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
    Assert(m_firstFormat == 0);
    return m_buffer;
}

template< size_t size >
format<size>::operator char* ()
{
    Assert(m_firstFormat == 0);
    return m_buffer;
}

template< size_t size >
const char* format<size>::c_str() const
{
    Assert(m_firstFormat == 0);
    return m_buffer;
}

template< size_t size >
format<size>& format<size>::operator|(char value)
{
    Assert(m_firstFormat);
    Assert(m_firstFormat[0] == '%');
    Assert(m_firstFormat[1] == 'c');
    m_firstFormat[0] = value;
    memmove(m_firstFormat+1, m_firstFormat+2, size-(m_firstFormat+2-m_buffer));
    m_firstFormat++;
    findToken();
    return *this;
}

template< size_t size >
format<size>& format<size>::operator|(const char* value)
{
    Assert(m_firstFormat);
    Assert(m_firstFormat[0] == '%');
    Assert(m_firstFormat[1] == 's');
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
format<size>& format<size>::operator|(const void* value)
{
    Assert(m_firstFormat);
    Assert(m_firstFormat[0] == '%');
    Assert(m_firstFormat[1] == 'p');
    static const size_t s = 2+sizeof(value)*2;
    char result[s];
    result[0] = '0';
    result[1] = 'x';
    for(int i = 0; i < sizeof(value)*2; ++i)
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
format<size>& format<size>::operator|(i64 value)
{
    Assert(m_firstFormat);
    Assert(m_firstFormat[0] == '%');
    Assert(m_firstFormat[1] == 'd');
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
format<size>& format<size>::operator|(u64 value)
{
    Assert(m_firstFormat);
    Assert(m_firstFormat[0] == '%');
    Assert(m_firstFormat[1] == 'u');
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
format<size>& format<size>::operator|(i32 value)
{
    return *this | (i64)value;
}

template< size_t size >
format<size>& format<size>::operator|(u32 value)
{
    return *this | (u64)value;
}

template< size_t size >
format<size>& format<size>::operator|(i16 value)
{
    return *this | (i64)value;
}

template< size_t size >
format<size>& format<size>::operator|(u16 value)
{
    return *this | (u64)value;
}

}

/*****************************************************************************/
#endif
