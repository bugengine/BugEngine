/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTIPARSE_PARSECONTEXT_HH_
#define BE_RTTIPARSE_PARSECONTEXT_HH_
/**************************************************************************************************/
#include <bugengine/rtti-parse/stdafx.h>
#include <bugengine/rtti-ast/node/node.hh>
#include <bugengine/rtti-ast/node/object.hh>

template < typename T >
struct ParseResult
{
    BugEngine::RTTI::AST::ParseLocation location;
    T                                   value;
};

union YYSTYPE
{
    ParseResult< bool >                                                 bValue;
    ParseResult< i64 >                                                  iValue;
    ParseResult< double >                                               fValue;
    ParseResult< char >                                                 cValue;
    ParseResult< char* >                                                sValue;
    ParseResult< ref< BugEngine::RTTI::AST::Node >* >                   value;
    ParseResult< BugEngine::RTTI::AST::Parameter* >                     param;
    ParseResult< minitl::vector< BugEngine::RTTI::AST::Parameter >* >   param_list;
    ParseResult< minitl::vector< ref< BugEngine::RTTI::AST::Node > >* > value_list;
};
#define YYSTYPE_IS_DECLARED 1
#define YYSTYPE_IS_TRIVIAL  1

namespace BugEngine { namespace RTTI { namespace AST {

struct ParseContext
{
    enum Header
    {
        HeaderDone,
        HeaderAnyValue,
        HeaderObject
    };

    minitl::Allocator* arena;
    ref< Node >        result;
    const char*        bufferStart;
    const char*        bufferEnd;
    const char*        buffer;
    MessageList&       errors;
    ParseLocation      location;
    Header             parseHeader;

    ParseContext(minitl::Allocator& arena, const char* bufferStart, const char* bufferEnd,
                 MessageList& errors, bool expectObject = false, u32 lineStart = 0,
                 u32 columnStart = 0);
    ~ParseContext();
};

extern ParseContext* g_parseContext;

}}}  // namespace BugEngine::RTTI::AST

extern "C" int be_value_parse(BugEngine::RTTI::AST::ParseContext* context);

/**************************************************************************************************/
#endif
