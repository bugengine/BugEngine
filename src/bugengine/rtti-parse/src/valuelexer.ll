/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */
%{
#include    <bugengine/rtti-ast/stdafx.h>
#include    <ctype.h>
#include    <parsecontext.hh>

#define yylval  be_value_lval
#include "valueparser.hh"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4127 4244 4267 4996 4505)
#endif

#ifdef malloc
# undef malloc
#endif
#ifdef realloc
# undef realloc
#endif
#ifdef free
# undef free
#endif
#define malloc(x)    BugEngine::Arena::temporary().alloc(x, 4)
#define relloc(x,s)  BugEngine::Arena::temporary().realloc(x, s, 4)
#define free(x)      BugEngine::Arena::temporary().free(x)
#ifdef strdup
# undef strdup
#endif
#define strdup(x)    be_strdup(x)

static char *be_strdup(const char *src)
{
    size_t x = strlen(src);
    char* result = (char*)malloc(x+1);
    strncpy(result, src, x+1);
    return result;
}

static i64 strToInteger(const char *text, size_t l)
{
    bool negate = false;
    i64 result = 0;
    if (*text == '-')
    {
        negate = true;
        text++;
        l--;
    }
    for (size_t i = 0; i < l; ++i)
    {
        result = result * 10 + (text[i]-'0');
    }
    return negate?-result:result;
}

static double strToDouble(const char *text, size_t /*l*/)
{
    return strtod(text, 0);
}

static void update (int num)
{
    ::BugEngine::RTTI::AST::g_parseContext->location.update(num);
}
static void newline()
{
    ::BugEngine::RTTI::AST::g_parseContext->location.newline();
}

extern "C" int be_value_wrap()
{
    return 1;
}

#define YY_INPUT(buf,result,max_size)                                               \
        {                                                                           \
            using namespace ::BugEngine::RTTI::AST;                              \
            result = max_size > g_parseContext->bufferEnd - g_parseContext->buffer  \
                ? g_parseContext->bufferEnd - g_parseContext->buffer                \
                : max_size;                                                         \
            memcpy(buf, g_parseContext->buffer, result);                            \
            g_parseContext->buffer += result;                                       \
        }



%}

%option prefix="be_value_"
%option never-interactive
%option nounput

DIGIT           [0-9]
HEXDIGIT        [0-9A-Fa-f]
DIGITS          ({DIGIT}+)
SIGN            ("+"|"-")
ID              [A-Za-z_][0-9A-Za-z_<>]*

%%

%{
    using namespace ::BugEngine::RTTI::AST;
    if (g_parseContext->parseHeader == ParseContext::HeaderObject)
    {
        g_parseContext->parseHeader = ParseContext::HeaderDone;
        return TOK_EXPECT_OBJECT;
    }
    else if (g_parseContext->parseHeader == ParseContext::HeaderAnyValue)
    {
        g_parseContext->parseHeader = ParseContext::HeaderDone;
        return TOK_EXPECT_ANY;
    }
%}

true                                            { update(be_value_leng);
                                                  yylval.bValue.value = true;
                                                  yylval.bValue.location = ::BugEngine::RTTI::AST::g_parseContext->location;
                                                  return VAL_BOOLEAN; }
false                                           { update(be_value_leng);
                                                  yylval.bValue.value = false;
                                                  yylval.bValue.location = ::BugEngine::RTTI::AST::g_parseContext->location;
                                                  return VAL_BOOLEAN; }
{ID}                                            { update(be_value_leng);
                                                  yylval.sValue.value = be_strdup(be_value_text);
                                                  yylval.sValue.location = ::BugEngine::RTTI::AST::g_parseContext->location;
                                                  return TOK_ID; }
\"[^\r\n\"]*\"                                  { update(be_value_leng);
                                                  yylval.sValue.value = be_strdup(be_value_text+1);
                                                  yylval.sValue.value[be_value_leng-2] = 0;
                                                  yylval.sValue.location = ::BugEngine::RTTI::AST::g_parseContext->location;
                                                  return VAL_STRING; }
@\"[^\r\n\"]*\"                                 { update(be_value_leng);
                                                  yylval.sValue.value = be_strdup(be_value_text+2);
                                                  yylval.sValue.location = ::BugEngine::RTTI::AST::g_parseContext->location;
                                                  return VAL_FILENAME; }
-?[0-9]+                                        { update(be_value_leng);
                                                  yylval.iValue.value = strToInteger(be_value_text, be_value_leng);
                                                  yylval.iValue.location = ::BugEngine::RTTI::AST::g_parseContext->location;
                                                  return VAL_INTEGER; }
{DIGITS}("."{DIGITS}?)?([eE]{SIGN}?{DIGITS})?   { update(be_value_leng);
                                                  yylval.fValue.value = strToDouble(be_value_text, be_value_leng);
                                                  yylval.fValue.location = ::BugEngine::RTTI::AST::g_parseContext->location;
                                                  return VAL_FLOAT; }
"\n"                                            { (void)&yyinput; newline(); }
[ \r\t]+                                        { update(be_value_leng); }
\#[^\n]*\n                                      { update(be_value_leng); }
.                                               { update(be_value_leng);
                                                  yylval.cValue.value = *be_value_text;
                                                  yylval.cValue.location = ::BugEngine::RTTI::AST::g_parseContext->location;
                                                  return *be_value_text; }

%%

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#if (YY_FLEX_MINOR_VERSION < 5) || (YY_FLEX_MINOR_VERSION == 5 && YY_FLEX_SUBMINOR_VERSION < 5)
int be_value_lex_destroy()
{
    return 0;
}
#endif
