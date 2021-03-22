/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_INTROSPECT_DBCONTEXT_HH_
#define BE_INTROSPECT_DBCONTEXT_HH_
/**************************************************************************************************/
#include <bugengine/introspect/stdafx.h>
#include <bugengine/filesystem/folder.script.hh>
#include <bugengine/introspect/dbnamespace.hh>
#include <bugengine/introspect/node/node.hh>
#include <bugengine/meta/value.hh>

namespace BugEngine { namespace Meta { namespace AST {

struct Message
{
    typedef minitl::format< 512u > MessageType;
    weak< const Node >             owner;
    MessageType                    message;
    LogLevel                       severity;
    Message(weak< const Node > owner, const MessageType& message, LogLevel severity)
        : owner(owner)
        , message(message)
        , severity(severity)
    {
    }
};
typedef minitl::vector< Message > MessageList;

struct be_api(INTROSPECT) DbContext
{
    ref< Namespace > const rootNamespace;
    ref< Folder > const    rootFolder;
    MessageList            messages;
    u32                    errorCount;

    DbContext(minitl::Allocator & arena, ref< Folder > rootFolder);
    DbContext(minitl::Allocator & arena, ref< Namespace > ns, ref< Folder > rootFolder);
    void error(weak< const Node > owner, const Message::MessageType& error);
    void warning(weak< const Node > owner, const Message::MessageType& warning);
    void info(weak< const Node > owner, const Message::MessageType& info);
};

}}}  // namespace BugEngine::Meta::AST

/**************************************************************************************************/
#endif
