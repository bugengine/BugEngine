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
    ,   errors(arena)
    ,   warnings(arena)
{
}

DbContext::DbContext(minitl::Allocator& arena, const ifilename& filename,
                     ref<const Namespace> ns, ref<Folder> rootFolder)
    :   filename(filename)
    ,   rootNamespace(ns)
    ,   rootFolder(rootFolder)
    ,   errors(arena)
    ,   warnings(arena)
{
}

void DbContext::error(const ParseLocation& where, const ErrorType& error)
{
    Error e(where, error);
    errors.push_back(e);
}

void DbContext::warning(const ParseLocation& where, const ErrorType& error)
{
    Error e(where, error);
    warnings.push_back(e);
}

}}}

