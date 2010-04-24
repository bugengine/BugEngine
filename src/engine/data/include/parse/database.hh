/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_DATA_PARSE_DATABASE_HH_
#define BE_DATA_PARSE_DATABASE_HH_
/*****************************************************************************/
#include    <parse/node.hh>
#include    <rtti/namespace.hh>
#include    <core/memory/streams.hh>

extern int yyparse(void *param);


namespace BugEngine { namespace Data { namespace Parse
{

class Database
{
    friend int ::yyparse(void *param);
public:
    enum Visibility
    {
        Private,
        Protected,
        Package,
        Public,
        Export
    };
private:
    class DatabaseElement : public Node
    {
        friend class Database;
    private:
        typedef minitl::map< istring,std::pair< Visibility, ref<const Node> > >  ChildrenContainer;
        typedef minitl::map< istring, ref<DatabaseElement> >                     NamespaceContainer;
    private:
        istring const                m_name;
        weak<DatabaseElement> const  m_parent;
        NamespaceContainer           m_namespaces;
        ChildrenContainer            m_objects;
    public:
        DatabaseElement(const istring& name, weak<DatabaseElement> parent);
        ~DatabaseElement();

        void add(const istring& name, Visibility v, ref<const Node> value);
        weak<DatabaseElement> push(const istring &name);
    private:
        void  dolink(Context& context) const override;
        Value doeval(Context& context) const override;
    private:
        DatabaseElement& operator=(const DatabaseElement& other);
        DatabaseElement(const DatabaseElement& other);
    };
    ref<DatabaseElement>  m_root;
    weak<DatabaseElement> m_current;
public:
    Database();
    ~Database();

    void add(const istring& name, Visibility v, ref<const Node> value);
    void push(const istring& name);
    void pop(const istring& name);

    void parse(weak<IMemoryStream> stream);
    ref<RTTI::Namespace> commit();
};

}}}

/*****************************************************************************/
#endif
