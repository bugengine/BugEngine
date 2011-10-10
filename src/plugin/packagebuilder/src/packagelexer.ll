%{
#include <stdafx.h>
#include <ctype.h>

#define yylval  be_package_lval
#include "packageparser.hh"

#pragma warning(disable:4127 4244 4267 4996 4505)

i64 strToInteger(const char *text)
{
    bool negate = false;
    i64 result = 0;
    if (*text == '-')
    {
        negate = true;
        text++;
    }
    while (isdigit(*text))
    {
        result = result * 10 + (*text-'0');
    }
    return negate?-result:result;
}

extern int g_packageLine;
extern int g_packageColumnBefore;
extern int g_packageColumnAfter;

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

extern const BugEngine::Allocator::Block<u8>* g_buffer;
extern int g_bufferPosition;
#define YY_INPUT(buf,result,max_size)                               \
        {                                                           \
            int size = max_size > g_buffer->count()-g_bufferPosition\
                ? g_buffer->count()-g_bufferPosition                \
                : max_size;                                         \
            memcpy(buf, g_buffer->data()+g_bufferPosition, size);   \
            result = size;                                          \
        }



%}

%option prefix="be_package_"
%option nounput

alpha       [A-Za-z_]
num         [0-9]
alphanum    [A-Za-z_0-9]
name        {alpha}{alphanum}*

%%

'-'?{num}+          { size_t size = strlen(yytext); update(size); yylval.iValue = strToInteger(yytext); return VAL_INTEGER; }
true                { update(4); yylval.bValue = true; return VAL_BOOLEAN; }
false               { update(5); yylval.bValue = false; return VAL_BOOLEAN; }
import              { update(6); return KW_import; }
{name}              { update(strlen(yytext)); yylval.sValue = strdup(yytext); return TOK_ID; }
"\n"                { newline(); }
[ \r\t]+            { update(strlen(yytext)); }
.                   { update(1); return *yytext; }

%%

static struct Cleanup
{
    Cleanup()
    {
    }
    ~Cleanup()
    {
        if (YY_CURRENT_BUFFER)
        {
            yy_delete_buffer(YY_CURRENT_BUFFER);
        }
    }
} cleanup;
