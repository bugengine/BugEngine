/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTI_PARSE_PARSECONTEXT_HH_
#define BE_RTTI_PARSE_PARSECONTEXT_HH_
/**************************************************************************************************/
#include <bugengine/rtti-parse/stdafx.h>
#include <bugengine/rtti-ast/dbcontext.hh>
#include <bugengine/rtti-ast/node/node.hh>
#include <bugengine/rtti-ast/node/object.hh>
#include <bugengine/rtti-parse/location.script.hh>

template < typename T >
struct ParseResult
{
    BugEngine::RTTI::Parse::Location location;
    T                                value;
};

union YYSTYPE
{
    ParseResult< bool >                                                      bValue;
    ParseResult< i64 >                                                       iValue;
    ParseResult< double >                                                    fValue;
    ParseResult< char >                                                      cValue;
    ParseResult< char* >                                                     sValue;
    ParseResult< ref< BugEngine::RTTI::AST::Node >* >                        value;
    ParseResult< ref< BugEngine::RTTI::AST::Parameter >* >                   param;
    ParseResult< minitl::vector< ref< BugEngine::RTTI::AST::Parameter > >* > param_list;
    ParseResult< minitl::vector< ref< BugEngine::RTTI::AST::Node > >* >      value_list;
};
#define YYSTYPE_IS_DECLARED 1
#define YYSTYPE_IS_TRIVIAL  1

namespace BugEngine { namespace RTTI { namespace Parse {

struct ParseContext
{
    minitl::Allocator* arena;
    ref< AST::Node >   result;
    const char*        bufferStart;
    const char*        bufferEnd;
    const char*        buffer;
    AST::MessageList&  errors;
    Location           location;

    ParseContext(minitl::Allocator& arena, const char* bufferStart, const char* bufferEnd,
                 AST::MessageList& errors, u32 lineStart = 0, u32 columnStart = 0);
    ~ParseContext();
};

extern ParseContext* g_parseContext;

}}}  // namespace BugEngine::RTTI::Parse

extern "C" int be_value_parse(BugEngine::RTTI::Parse::ParseContext* context);
extern "C" int be_value_lex_destroy();

/**************************************************************************************************/
#endif
