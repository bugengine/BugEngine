/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_NAMESPACE_HH_
#define BE_RTTI_NAMESPACE_HH_
/*****************************************************************************/
#include    <rtti/object.hh>
#include    <rtti/property.hh>
#include    <rtti/metaclass.hh>
#include    <core/utils/singleton.hh>

#include    <rtti/value.hh>

namespace BugEngine { namespace RTTI
{

class RTTIEXPORT Namespace : public Object
{
protected:
    class MetaClass : public RTTI::MetaClass
    {
    private:
        class Property : public RTTI::Property
        {
        private:
            mutable Value   m_value;
        public:
            Property();
            ~Property();

            virtual bool    readable(Object* from) const override;
            virtual bool    writable(Object* from) const override;
            virtual void    set(Object* dest, const Value& value) const override;
            virtual Value   get(Object* from) const override;
        };
    public:
        MetaClass();
        ~MetaClass();

        void    set(const istring& name, const Value& value);
        Value   get(const istring& name);
        void    erase(const istring& name);
    };
private:
    typedef minitl::map< istring, refptr<Namespace> >  NamespaceMap;
    refptr<MetaClass>   m_metaClass;
    NamespaceMap        m_subnamespaces;
public:
    Namespace();
    virtual ~Namespace();

    virtual const MetaClass*  metaclass() const override;

    void                set(const istring& ns, const Value& value);
    Value               get(const istring& name) const;
    void                mount(const istring& name, refptr<Namespace> ns);
    void                umount(const istring& name);
    refptr<Namespace>   getNamespace(const istring& name);
    refptr<Namespace>   createNamespace(const istring& name);

    void insert(const inamespace& ns, const Value& value);
    void insert(const inamespace& ns, Object* value);
    void insert(const inamespace& ns, refptr<Object> value);
    Value get(const inamespace& ns);
    void clear();

    static refptr<Namespace> root();
private:
    Namespace(const Namespace& other);
    Namespace& operator=(const Namespace& other);
};

}}

/*****************************************************************************/
#endif
