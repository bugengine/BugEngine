/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_AST_DBCONTEXT_HH_
#define BE_RTTI_AST_DBCONTEXT_HH_
/**************************************************************************************************/
#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/filesystem/folder.script.hh>
#include <bugengine/rtti-ast/dbnamespace.hh>
#include <bugengine/rtti/value.hh>

namespace BugEngine { namespace RTTI { namespace AST {

struct ParseLocation
{
    u32 line;
    u32 columnStart;
    u32 columnEnd;

    void newline()
    {
        line++;
        columnStart = 0;
        columnEnd   = 1;
    }
    void update(u32 column)
    {
        columnStart = columnEnd;
        columnEnd += column;
    }
};

struct Message
{
    typedef minitl::format< 512u > MessageType;
    ParseLocation                  location;
    MessageType                    message;
    LogLevel                       severity;
    Message(const ParseLocation& location, const MessageType& message, LogLevel severity)
        : location(location)
        , message(message)
        , severity(severity)
    {
    }
};
typedef minitl::vector< Message > MessageList;

struct be_api(RTTI_AST) DbContext
{
    const ifilename        filename;
    ref< Namespace > const rootNamespace;
    ref< Folder > const    rootFolder;
    MessageList            messages;
    u32                    errorCount;

    DbContext(minitl::Allocator & arena, const ifilename& filename, ref< Folder > rootFolder);
    DbContext(minitl::Allocator & arena, const ifilename& filename, ref< Namespace > ns,
              ref< Folder > rootFolder);
    void error(const ParseLocation& location, const Message::MessageType& error);
    void warning(const ParseLocation& location, const Message::MessageType& warning);
};

}}}  // namespace BugEngine::RTTI::AST

/**************************************************************************************************/
#endif
