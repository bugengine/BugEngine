/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTIPARSE_DBCONTEXT_HH_
#define BE_RTTIPARSE_DBCONTEXT_HH_
/**************************************************************************************************/
#include <bugengine/rttiparse/stdafx.h>
#include <bugengine/filesystem/folder.script.hh>
#include <bugengine/rtti/value.hh>
#include <bugengine/rttiparse/dbnamespace.hh>

namespace BugEngine { namespace RTTI { namespace Parser {

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

struct be_api(RTTIPARSE) DbContext
{
    const ifilename              filename;
    ref< const Namespace > const rootNamespace;
    ref< Folder > const          rootFolder;
    MessageList                  messages;
    u32                          errorCount;

    DbContext(minitl::Allocator & arena, const ifilename& filename, ref< Folder > rootFolder);
    DbContext(minitl::Allocator & arena, const ifilename& filename, ref< const Namespace > ns,
              ref< Folder > rootFolder);
    void error(const ParseLocation& location, const Message::MessageType& error);
    void warning(const ParseLocation& location, const Message::MessageType& warning);
};

}}}  // namespace BugEngine::RTTI::Parser

/**************************************************************************************************/
#endif
