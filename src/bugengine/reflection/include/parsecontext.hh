/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_META_PARSE_PARSECONTEXT_HH_
#define BE_META_PARSE_PARSECONTEXT_HH_
/**************************************************************************************************/
#include <bugengine/reflection/stdafx.h>
#include <bugengine/introspect/dbcontext.hh>
#include <bugengine/introspect/node/node.hh>
#include <bugengine/introspect/node/object.hh>
#include <bugengine/reflection/location.script.hh>

template < typename T >
struct ParseResult
{
    BugEngine::Meta::Parse::Location location;
    T                                value;
};

union YYSTYPE
{
    ParseResult< bool >                                                      bValue;
    ParseResult< i64 >                                                       iValue;
    ParseResult< double >                                                    fValue;
    ParseResult< char >                                                      cValue;
    ParseResult< char* >                                                     sValue;
    ParseResult< ref< BugEngine::Meta::AST::Node >* >                        value;
    ParseResult< ref< BugEngine::Meta::AST::Parameter >* >                   param;
    ParseResult< minitl::vector< ref< BugEngine::Meta::AST::Parameter > >* > param_list;
    ParseResult< minitl::vector< ref< BugEngine::Meta::AST::Node > >* >      value_list;
};
#define YYSTYPE_IS_DECLARED 1
#define YYSTYPE_IS_TRIVIAL  1

namespace BugEngine { namespace Meta { namespace Parse {

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

}}}  // namespace BugEngine::Meta::Parse

extern "C" int be_value_parse(BugEngine::Meta::Parse::ParseContext* context);
extern "C" int be_value_lex_destroy();

/**************************************************************************************************/
#endif
