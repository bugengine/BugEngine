%{
#include    <stdafx.h>
#include    <buildcontext.hh>

#define yyparse be_package_parse
#define yylex   be_package_lex
#define yyerror be_package_error
#define yylval  be_package_lval
#define yychar  be_package_char
#define yydebug be_package_debug
#define yynerrs be_package_nerrs


#define YYPARSE_PARAM   param
#include    <stdafx.h>

#ifdef _MSC_VER
# include <malloc.h>
# pragma warning(push)
# pragma warning(disable:4065) // switch contains 'default' but no 'case' label
# pragma warning(disable:4244) // conversion from 'type1' to 'type2' : possible loss of data
# pragma warning(disable:4100) // param : unreferenced formal parameter
# pragma warning(disable:4702) // unreachable code
#endif

#define YYSTACK_USE_ALLOCA 1

extern int yylex();

static int yyerror(const char *msg)
{
    be_error("Error parsing %d (%d:%d): %s" | (g_packageLine+1) | (g_packageColumnBefore+1) | (g_packageColumnAfter+1) | msg);
    return 0;
}

#ifndef __attribute__
# define __attribute__(x)
#endif
%}

%token  TOK_ID
%token  VAL_STRING VAL_INTEGER VAL_FLOAT VAL_BOOLEAN

%token  KW_import KW_plugin

%type   <bValue>    VAL_BOOLEAN
%type   <iValue>    VAL_INTEGER
%type   <fValue>    VAL_FLOAT
%type   <sValue>    VAL_STRING
%type   <sValue>    TOK_ID

%union
{
    bool    bValue;
    i64     iValue;
    double  fValue;
    char*   sValue;
}

%start  file

%%

file:
        file
        decl
    |
        /* empty */
    ;

decl:
        decl_import
    |
        decl_plugin
    |
        decl_object
    ;

decl_import:
        KW_import fullname';'
    ;

decl_plugin:
        KW_plugin TOK_ID';'
            {
                BugEngine::istring i($2);
                BugEngine::Plugin<minitl::pointer> plugin (i, BugEngine::Plugin<minitl::pointer>::Preload);
                ((BugEngine::PackageBuilder::BuildContext*)param)->plugins.insert(std::make_pair(i, plugin));
                free($2);
            }
    ;

decl_object:
        TOK_ID '=' fullname '('
            params
        ')'
    ;

params:
        params
        param
    |
        /* empty */
    ;

param:
        TOK_ID '=' value ';'
    ;

value:
        fullname
    |
        VAL_BOOLEAN
    |
        VAL_INTEGER
    |
        VAL_FLOAT
    |
        VAL_STRING
    ;

fullname:
        TOK_ID
    |
        TOK_ID '.' fullname
    ;

%%


