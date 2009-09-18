/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_DATA_PARSE_CONTEXT_HH_
#define BE_DATA_PARSE_CONTEXT_HH_
/*****************************************************************************/

namespace BugEngine { namespace Data { namespace Parse
{

class Database;

class Context
{
    friend class Database;
private:
    Database&               m_database;
private:
    Context(Database& db);
public:
    ~Context();

    Database& database();
    const Database& database() const;

    void warning(const ifilename& file, int line, int column, const char *message);
    void error(const ifilename& file, int line, int column, const char *message);
private:
    Context& operator=(const Context& other);
    Context(const Context& other);
};

}}}

/*****************************************************************************/
#endif
