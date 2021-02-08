/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/dbcontext.hh>

namespace BugEngine { namespace RTTI { namespace AST {

DbContext::DbContext(minitl::Allocator& arena, ref< Folder > rootFolder)
    : rootNamespace(ref< Namespace >::create(arena, byref(arena)))
    , rootFolder(rootFolder)
    , messages(arena)
    , errorCount()
{
}

DbContext::DbContext(minitl::Allocator& arena, ref< Namespace > ns, ref< Folder > rootFolder)
    : rootNamespace(ns)
    , rootFolder(rootFolder)
    , messages(arena)
    , errorCount()
{
}

void DbContext::error(weak< const Node > owner, const Message::MessageType& error)
{
    messages.push_back(Message(owner, error, logError));
    errorCount++;
}

void DbContext::warning(weak< const Node > owner, const Message::MessageType& warning)
{
    messages.push_back(Message(owner, warning, logWarning));
}

void DbContext::info(weak< const Node > owner, const Message::MessageType& info)
{
    messages.push_back(Message(owner, info, logInfo));
}

}}}  // namespace BugEngine::RTTI::AST
