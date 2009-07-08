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

#ifndef BE_RTTI_STRING_MARSHALLER_HH_
#define BE_RTTI_STRING_MARSHALLER_HH_
/*****************************************************************************/
#include    <rtti/marshallers/abstract.hh>
#include    <rtti/value.hh>

namespace BugEngine { namespace RTTI
{

template< >
class RTTIEXPORT Marshaller<std::string> : public AbstractMarshaller
{
public:
    virtual ValueTypeIndex type() const;
    virtual ValueCopy   get(void* source)                                       const override;
    virtual void        set(const Value& value, void* dst)                      const override;

    inline ValueCopy    castfrom(const std::string& value)                      const;
    inline std::string  castto(const Value& value)                              const;
};


template< >
class RTTIEXPORT Marshaller<istring> : public AbstractMarshaller
{
public:
    virtual ValueTypeIndex type() const;
    virtual ValueCopy   get(void* source)                                       const override;
    virtual void        set(const Value& value, void* dst)                      const override;

    inline ValueCopy    castfrom(const istring& value)                          const;
    inline istring      castto(const Value& value)                              const;
};

template< >
class RTTIEXPORT Marshaller<inamespace> : public AbstractMarshaller
{
public:
    virtual ValueTypeIndex type() const;
    virtual ValueCopy   get(void* source)                                       const override;
    virtual void        set(const Value& value, void* dst)                      const override;

    inline ValueCopy    castfrom(const inamespace& value)                          const;
    inline inamespace   castto(const Value& value)                              const;
};

template< >
class RTTIEXPORT Marshaller<ifilename> : public AbstractMarshaller
{
public:
    virtual ValueTypeIndex type() const;
    virtual ValueCopy   get(void* source)                                       const override;
    virtual void        set(const Value& value, void* dst)                      const override;

    inline ValueCopy    castfrom(const ifilename& value)                          const;
    inline ifilename    castto(const Value& value)                              const;
};

template< >
class RTTIEXPORT Marshaller<ipath> : public AbstractMarshaller
{
public:
    virtual ValueTypeIndex type() const;
    virtual ValueCopy   get(void* source)                                       const override;
    virtual void        set(const Value& value, void* dst)                      const override;

    inline ValueCopy    castfrom(const ipath& value)                          const;
    inline ipath        castto(const Value& value)                              const;
};

}}

#include    <rtti/marshallers/string.inl>

/*****************************************************************************/
#endif
