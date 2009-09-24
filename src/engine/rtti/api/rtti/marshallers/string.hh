/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_STRING_MARSHALLER_HH_
#define BE_RTTI_STRING_MARSHALLER_HH_
/*****************************************************************************/
#include    <rtti/marshallers/abstract.hh>
#include    <rtti/value.hh>

namespace BugEngine { namespace RTTI
{

template< >
class be_api(RTTI) Marshaller<std::string> : public AbstractMarshaller
{
public:
    virtual ValueTypeIndex type() const;
    virtual ValueCopy   get(void* source)                                       const override;
    virtual void        set(const Value& value, void* dst)                      const override;

    inline ValueCopy    castfrom(const std::string& value)                      const;
    inline std::string  castto(const Value& value)                              const;
};


template< >
class be_api(RTTI) Marshaller<istring> : public AbstractMarshaller
{
public:
    virtual ValueTypeIndex type() const;
    virtual ValueCopy   get(void* source)                                       const override;
    virtual void        set(const Value& value, void* dst)                      const override;

    inline ValueCopy    castfrom(const istring& value)                          const;
    inline istring      castto(const Value& value)                              const;
};

template< >
class be_api(RTTI) Marshaller<inamespace> : public AbstractMarshaller
{
public:
    virtual ValueTypeIndex type() const;
    virtual ValueCopy   get(void* source)                                       const override;
    virtual void        set(const Value& value, void* dst)                      const override;

    inline ValueCopy    castfrom(const inamespace& value)                          const;
    inline inamespace   castto(const Value& value)                              const;
};

template< >
class be_api(RTTI) Marshaller<ifilename> : public AbstractMarshaller
{
public:
    virtual ValueTypeIndex type() const;
    virtual ValueCopy   get(void* source)                                       const override;
    virtual void        set(const Value& value, void* dst)                      const override;

    inline ValueCopy    castfrom(const ifilename& value)                          const;
    inline ifilename    castto(const Value& value)                              const;
};

template< >
class be_api(RTTI) Marshaller<ipath> : public AbstractMarshaller
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
