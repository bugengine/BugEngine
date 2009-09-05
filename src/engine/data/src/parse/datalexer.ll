%{
#include <data/stdafx.h>

#include <parse/database.hh>
#include <parse/node.hh>
#include <parse/reference.hh>
#include <parse/immediate.hh>
#include <parse/type.hh>

using namespace BugEngine::Data::Parse;

#include "dataparser.hh"


#pragma warning(disable:4127 4244 4267 4996 4505)

#include <core/memory/streams.hh>

namespace BugEngine { namespace Data { namespace Parse
{

BugEngine::AbstractMemoryStream* g_parseStream = 0;

}}}

#define YY_INPUT(buf,result,max_size)           \
    result = g_parseStream->read(buf,max_size);

extern int g_line;
extern int g_columnbefore;
extern int g_columnafter;

static void update (int num)
{
    g_columnbefore = g_columnafter;
    g_columnafter += num;
}

static void newline()
{
    g_line++;
    g_columnbefore = 0;
    g_columnafter = 1;
}

#ifndef _MSC_VER
# define atoi64(a,l) strtoll(a,0,l)
#else
# define atoi64(a,l) _atoi64(a)
#endif

%}
%option noyywrap
%option nounput

alpha       [A-Za-z_]
num         [0-9]
alphanum    [A-Za-z_0-9]
name        {alpha}{alphanum}*

%%

{num}+              { size_t size = strlen(yytext); update(size); yylval.intval = atoi64(yytext,size); return VAL_int; }
true                { update(4); yylval.boolval = true; return VAL_bool; }
false               { update(5); yylval.boolval = false; return VAL_bool; }
class               { update(5); return KW_class; }
any                 { update(3); return KW_any; }
bool                { update(4); return KW_bool; }
int                 { update(3); return KW_int; }
float               { update(5); return KW_float; }
string              { update(6); return KW_string; }
vector              { update(6); return KW_vector; }
map                 { update(3); return KW_map; }
typeof              { update(6); return KW_typeof; }
private             { update(7); return KW_private; }
protected           { update(9); return KW_protected; }
package             { update(7); return KW_package; }
public              { update(6); return KW_public; }
export              { update(6); return KW_export; }
namespace           { update(9); return KW_namespace; }
{name}              { update(strlen(yytext)); yylval.strval = strdup(yytext); return VAL_id; }
\"[^\"]*\"          { update(strlen(yytext)); yylval.strval = strdup(yytext+1); yylval.strval[yyleng-2] = '\0'; return VAL_string; }
[ \r\t]+            { update(strlen(yytext)); }
"\n"                { newline(); }
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
