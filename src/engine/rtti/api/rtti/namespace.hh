/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_NAMESPACE_HH_
#define BE_RTTI_NAMESPACE_HH_
/*****************************************************************************/
#include    <rtti/object.hh>
#include    <rtti/property.hh>
#include    <rtti/metaclass.hh>

#include    <rtti/value.hh>

namespace BugEngine { namespace RTTI
{

class be_api(RTTI) Namespace : public Object
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

            virtual bool    readable(weak<Object> from) const override;
            virtual bool    writable(weak<Object> from) const override;
            virtual void    set(weak<Object> dest, const Value& value) const override;
            virtual Value   get(weak<Object> from) const override;
        };
    public:
        MetaClass();
        ~MetaClass();

        weak<const RTTI::Property>  set(const istring& name, const Value& value);
        Value                       get(const istring& name);
        void                        erase(const istring& name);
    };
private:
    typedef minitl::map< istring, ref<Namespace> >  NamespaceMap;
    ref<MetaClass>  m_metaClass;
    NamespaceMap    m_subnamespaces;
public:
    Namespace();
    virtual ~Namespace();

    virtual ref<const RTTI::MetaClass>  metaclass() const override;

    weak<const RTTI::Property>  set(const istring& ns, const Value& value);
    Value                       get(const istring& name) const;
    void                        mount(const istring& name, ref<Namespace> ns);
    void                        umount(const istring& name);
    ref<Namespace>              getNamespace(const istring& name);
    ref<Namespace>              createNamespace(const istring& name);

    weak<const RTTI::Property>  insert(const inamespace& ns, const Value& value);
    weak<const RTTI::Property>  insert(const inamespace& ns, weak<Object> value);
    weak<const RTTI::Property>  insert(const inamespace& ns, ref<Object> value);
    Value get(const inamespace& ns);
    void clear();

    static ref<Namespace> root();
private:
    Namespace(const Namespace& other);
    Namespace& operator=(const Namespace& other);
};

}}

/*****************************************************************************/
#endif
