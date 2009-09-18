/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_OBJECT_MARSHALLER_HH_
#define BE_RTTI_OBJECT_MARSHALLER_HH_
/*****************************************************************************/
#include    <rtti/marshallers/abstract.hh>
#include    <rtti/value.hh>

namespace BugEngine { namespace RTTI
{

template< typename T >
class Marshaller< refptr<T> > : public AbstractMarshaller
{
public:
    virtual ValueTypeIndex type() const;
    virtual ValueCopy   get(void* source)                                       const override;
    virtual ValueCopy   get(void* source, size_t index)                         const override;
    virtual ValueCopy   get(void* source, const Value& key)                     const override;
    virtual void        set(const Value& value, void* dst)                      const override;
    virtual void        set(const Value& value, void* dst, size_t index)        const override;
    virtual void        set(const Value& value, void* dst, const Value& key)    const override;
    virtual void        push_back(const Value& value, void* dst)                const override;
    virtual void        push_front(const Value& value, void* dst)               const override;
    virtual void        pop_back(const Value& value, void* dst)                 const override;
    virtual void        pop_front(const Value& value, void* dst)                const override;

    inline ValueCopy    castfrom(refptr<T> value)                               const;
    inline refptr<T>    castto(const Value& value)                              const;
};


template< typename T >
class Marshaller<T*> : public AbstractMarshaller
{
public:
    virtual ValueTypeIndex type() const;
    virtual ValueCopy   get(void* source)                                       const override;
    virtual ValueCopy   get(void* source, size_t index)                         const override;
    virtual ValueCopy   get(void* source, const Value& key)                     const override;
    virtual void        set(const Value& value, void* dst)                      const override;
    virtual void        set(const Value& value, void* dst, size_t index)        const override;
    virtual void        set(const Value& value, void* dst, const Value& key)    const override;
    virtual void        push_back(const Value& value, void* dst)                const override;
    virtual void        push_front(const Value& value, void* dst)               const override;
    virtual void        pop_back(const Value& value, void* dst)                 const override;
    virtual void        pop_front(const Value& value, void* dst)                const override;

    inline ValueCopy    castfrom(T* value)                                      const;
    inline T*           castto(const Value& value)                              const;
};

}}

#include    <rtti/marshallers/object.inl>

/*****************************************************************************/
#endif
