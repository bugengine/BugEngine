/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTIPARSE_PARSECONTEXT_HH_
#define BE_RTTIPARSE_PARSECONTEXT_HH_
/**************************************************************************************************/
#include    <rttiparse/stdafx.h>
#include    <rttiparse/node.hh>
#include    <rttiparse/object.hh>


template< typename T >
struct ParseResult
{
    BugEngine::RTTI::Parser::ParseLocation  location;
    T                                       value;
};

union YYSTYPE
{
    ParseResult<bool>                                                       bValue;
    ParseResult<i64>                                                        iValue;
    ParseResult<double>                                                     fValue;
    ParseResult<char>                                                       cValue;
    ParseResult<char*>                                                      sValue;
    ParseResult< ref<BugEngine::RTTI::Parser::Node>* >                      value;
    ParseResult< BugEngine::RTTI::Parser::Parameter* >                      param;
    ParseResult< minitl::vector< BugEngine::RTTI::Parser::Parameter >* >    param_list;
    ParseResult< minitl::vector< ref<BugEngine::RTTI::Parser::Node> >* >    value_list;
};
#define YYSTYPE_IS_DECLARED 1
#define YYSTYPE_IS_TRIVIAL 1



namespace BugEngine { namespace RTTI { namespace Parser
{

struct ParseContext
{
    enum Header
    {
        HeaderDone,
        HeaderAnyValue,
        HeaderObject
    };

    minitl::Allocator*  arena;
    ref<Node>           result;
    const char*         bufferStart;
    const char*         bufferEnd;
    const char*         buffer;
    ErrorList&          errors;
    ParseLocation       location;
    Header              parseHeader;

    ParseContext(minitl::Allocator& arena,
                 const char*        bufferStart,
                 const char*        bufferEnd,
                 ErrorList&         errors,
                 bool               expectObject = false,
                 u32                lineStart = 0,
                 u32                columnStart = 0);
    ~ParseContext();
};

extern ParseContext* g_parseContext;

}}}

extern "C" int be_value_parse(BugEngine::RTTI::Parser::ParseContext* context);

/**************************************************************************************************/
#endif
