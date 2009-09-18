/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_DATA_PARSE_DATABASE_HH_
#define BE_DATA_PARSE_DATABASE_HH_
/*****************************************************************************/
#include    <parse/node.hh>
#include    <rtti/namespace.hh>

extern int yyparse(void *param);


namespace BugEngine { namespace Data { namespace Parse
{

class Database;

struct ParseParam
{
    const ifilename&    m_filename;
    Database*           m_database;
    ParseParam(const ifilename& file, Database* db) :
        m_filename(file), m_database(db)
    {}
private:
    ParseParam(const ParseParam& other);
    ParseParam& operator=(const ParseParam& other);
};

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
        typedef minitl::map< istring,std::pair< Visibility, refptr<const Node> > >  ChildrenContainer;
        typedef minitl::map< istring, refptr<DatabaseElement> >                     NamespaceContainer;
    private:
        istring const           m_name;
        DatabaseElement* const  m_parent;
        NamespaceContainer      m_namespaces;
        ChildrenContainer       m_objects;
    public:
        DatabaseElement(const istring& name, DatabaseElement* parent);
        ~DatabaseElement();

        void add(const istring& name, Visibility v, refptr<const Node> value);
        DatabaseElement* push(const istring &name);
    private:
        void  dolink(Context& context) const override;
        Value doeval(Context& context) const override;
    private:
        DatabaseElement& operator=(const DatabaseElement& other);
        DatabaseElement(const DatabaseElement& other);
    };
    refptr<DatabaseElement> m_root;
    DatabaseElement*        m_current;
public:
    Database();
    ~Database();

    void add(const istring& name, Visibility v, refptr<const Node> value);
    void push(const istring& name);
    void pop(const istring& name);

    void parse(const ifilename& file);
    refptr<RTTI::Namespace> commit();
};

}}}

/*****************************************************************************/
#endif
