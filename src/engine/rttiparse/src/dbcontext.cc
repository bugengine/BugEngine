/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <rttiparse/stdafx.h>
#include    <rttiparse/dbcontext.hh>
#include    <rttiparse/valueparse.hh>


namespace BugEngine { namespace RTTI { namespace Parser
{


DbContext::DbContext(minitl::Allocator& arena, const ifilename& filename, ref<Folder> rootFolder)
    :   filename(filename)
    ,   rootNamespace(ref<Namespace>::create(arena, byref(arena)))
    ,   rootFolder(rootFolder)
    ,   messages(arena)
    ,   errorCount()
{
}

DbContext::DbContext(minitl::Allocator& arena, const ifilename& filename,
                     ref<const Namespace> ns, ref<Folder> rootFolder)
    :   filename(filename)
    ,   rootNamespace(ns)
    ,   rootFolder(rootFolder)
    ,   messages(arena)
    ,   errorCount()
{
}

void DbContext::error(const ParseLocation& location, const Message::MessageType& error)
{
    messages.push_back(Message(location, error, logError));
    errorCount++;
}

void DbContext::warning(const ParseLocation& location, const Message::MessageType& warning)
{
    messages.push_back(Message(location, warning, logWarning));
}

}}}

