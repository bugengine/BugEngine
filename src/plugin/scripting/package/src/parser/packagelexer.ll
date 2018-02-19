/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */
%{
#include    <package/stdafx.h>
#include    <ctype.h>
#include    <buildcontext.hh>
#include    <package/nodes/value.hh>

using namespace BugEngine::PackageBuilder::Nodes;

#define yylval  be_package_lval
#include "packageparser.hh"

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

i64 strToInteger(const char *text, size_t l)
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

double strToDouble(const char *text, size_t /*l*/)
{
    return strtod(text, 0);
}

static void update (int num)
{
    g_packageColumnBefore = g_packageColumnAfter;
    g_packageColumnAfter += num;
}
static void newline()
{
    g_packageLine++;
    g_packageColumnBefore = 0;
    g_packageColumnAfter = 1;
}

extern "C" int be_package_wrap()
{
    return 1;
}

#define YY_INPUT(buf,result,max_size)                               \
        {                                                           \
            result = max_size > g_buffer->count()-g_bufferPosition  \
                ? g_buffer->count()-g_bufferPosition                \
                : max_size;                                         \
            memcpy(buf, g_buffer->data()+g_bufferPosition, result); \
            g_bufferPosition += result;                             \
        }



%}

%option prefix="be_package_"
%option never-interactive
%option nounput

DIGIT         [0-9]
HEXDIGIT      [0-9A-Fa-f]
DIGITS        ({DIGIT}+)
SIGN          ("+"|"-")

%%

true                                                    { update(be_package_leng); yylval.bValue = true; return VAL_BOOLEAN; }
false                                                   { update(be_package_leng); yylval.bValue = false; return VAL_BOOLEAN; }
import                                                  { update(be_package_leng); return KW_import; }
load                                                    { update(be_package_leng); return KW_plugin; }
namespace                                               { update(be_package_leng); return KW_namespace; }
zip                                                     { update(be_package_leng); return KW_zip; }
as                                                      { update(be_package_leng); return KW_as; }
->                                                      { update(be_package_leng); return KW_notify; }
[0-9A-Za-z_\-\+\$]*[A-Za-z_\-\+\$]+[0-9A-Za-z_\-\+\$]*  { update(be_package_leng); yylval.sValue = be_strdup(be_package_text); return TOK_ID; }
\"[^\r\n\"]*\"                                          { update(be_package_leng); yylval.sValue = be_strdup(be_package_text+1); yylval.sValue[be_package_leng-2] = 0; return VAL_STRING; }
\<[^\r\n\"]*\>                                          { update(be_package_leng); yylval.sValue = be_strdup(be_package_text+1); yylval.sValue[be_package_leng-2] = 0; return VAL_FILENAME; }
-?[0-9]+                                                { update(be_package_leng); yylval.iValue = strToInteger(be_package_text, be_package_leng); return VAL_INTEGER; }
{DIGITS}("."{DIGITS}?)?([eE]{SIGN}?{DIGITS})?           { update(be_package_leng); yylval.fValue = strToDouble(be_package_text, be_package_leng); return VAL_FLOAT; }
"\n"                                                    { (void)&yyinput; newline(); }
[ \r\t]+                                                { update(be_package_leng); }
\#[^\n]*\n                                              { update(be_package_leng); }
.                                                       { update(be_package_leng); return *be_package_text; }

%%

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#if (YY_FLEX_MINOR_VERSION < 5) || (YY_FLEX_MINOR_VERSION == 5 && YY_FLEX_SUBMINOR_VERSION < 5)
int be_package_lex_destroy()
{
    return 0;
}
#endif
