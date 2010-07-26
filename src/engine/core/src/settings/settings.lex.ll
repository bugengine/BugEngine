%{
#include <core/stdafx.h>

#define yylval  be_settings_lval
#include "settings.bison.hh"

#pragma warning(disable:4127 4244 4267 4996 4505)

i64 strToInteger(const char *text)
{
    bool negate = false;
    i64 result = 0;
    if(*text == '-')
    {
        negate = true;
        text++;
    }
    while(isdigit(*text))
    {
        result = result * 10 + (*text-'0');
    }
    return negate?-result:result;
}

extern int g_settingsLine;
extern int g_settingsColumnBefore;
extern int g_settingsColumnAfter;

static void update (int num)
{
    g_settingsColumnBefore = g_settingsColumnAfter;
    g_settingsColumnAfter += num;
}
static void newline()
{
    g_settingsLine++;
    g_settingsColumnBefore = 0;
    g_settingsColumnAfter = 1;
}

extern "C" int be_settings_wrap()
{
    return 1;
}

#ifdef BE_PLATFORM_WIN32
static int isatty (int)
{
	return 0;
}
#endif

%}

%option prefix="be_settings_"
%option nounput

alpha       [A-Za-z_]
num         [0-9]
alphanum    [A-Za-z_0-9]
name        {alpha}{alphanum}*

%%

'-'?{num}+          { size_t size = strlen(yytext); update(size); yylval.iValue = strToInteger(yytext); return VAL_INTEGER; }
true                { update(4); yylval.bValue = true; return VAL_BOOLEAN; }
false               { update(5); yylval.bValue = false; return VAL_BOOLEAN; }
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
        if(YY_CURRENT_BUFFER)
        {
            yy_delete_buffer(YY_CURRENT_BUFFER);
        }
    }
} cleanup;
