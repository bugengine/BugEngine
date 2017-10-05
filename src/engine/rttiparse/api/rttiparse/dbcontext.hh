/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTIPARSE_DBCONTEXT_HH_
#define BE_RTTIPARSE_DBCONTEXT_HH_
/**************************************************************************************************/
#include    <rttiparse/stdafx.h>
#include    <rttiparse/dbnamespace.hh>
#include    <rtti/value.hh>
#include    <filesystem/folder.script.hh>


namespace BugEngine { namespace RTTI { namespace Parser
{

struct ParseLocation
{
    u32 line;
    u32 columnStart;
    u32 columnEnd;

    void newline()
    {
        line++;
        columnStart = 0;
        columnEnd = 1;
    }
    void update(u32 column)
    {
        columnStart = columnEnd;
        columnEnd += column;
    }
};

typedef minitl::format<512> ErrorType;

struct Error
{
    ParseLocation   location;
    ErrorType       errorMessage;
    Error(const ParseLocation& location,
          const ErrorType& message)
        :   location(location)
        ,   errorMessage(message)
    {
    }
};

typedef minitl::vector< Error > ErrorList;

struct be_api(RTTIPARSE) DbContext
{
    const ifilename             filename;
    ref<const Namespace> const  rootNamespace;
    ref<Folder> const           rootFolder;
    ErrorList                   errors;
    ErrorList                   warnings;

    DbContext(minitl::Allocator& arena, const ifilename& filename, ref<Folder> rootFolder);
    DbContext(minitl::Allocator& arena, const ifilename& filename,
              ref<const Namespace> ns, ref<Folder> rootFolder);
    void error(const ParseLocation& where, const ErrorType& error);
    void warning(const ParseLocation& where, const ErrorType& error);
};

}}}

/**************************************************************************************************/
#endif

