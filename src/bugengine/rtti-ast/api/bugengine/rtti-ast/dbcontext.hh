/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_AST_DBCONTEXT_HH_
#define BE_RTTI_AST_DBCONTEXT_HH_
/**************************************************************************************************/
#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/filesystem/folder.script.hh>
#include <bugengine/rtti-ast/dbnamespace.hh>
#include <bugengine/rtti-ast/node/node.hh>
#include <bugengine/rtti/value.hh>

namespace BugEngine { namespace RTTI { namespace AST {

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

struct be_api(RTTI_AST) DbContext
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

}}}  // namespace BugEngine::RTTI::AST

/**************************************************************************************************/
#endif
