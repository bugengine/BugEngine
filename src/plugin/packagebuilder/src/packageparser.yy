%{
#include    <stdafx.h>
#include    <buildcontext.hh>
#include    <packagebuilder/nodes/package.hh>
#include    <packagebuilder/nodes/object.hh>
#include    <packagebuilder/nodes/reference.hh>

#define yyparse be_package_parse
#define yylex   be_package_lex
#define yyerror be_package_error
#define yylval  be_package_lval
#define yychar  be_package_char
#define yydebug be_package_debug
#define yynerrs be_package_nerrs


#define YYPARSE_PARAM   param

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
    be_error("%s at line %d (%d:%d)" | msg | (g_packageLine+1) | (g_packageColumnBefore+1) | (g_packageColumnAfter+1));
    return 0;
}

#ifndef __attribute__
# define __attribute__(x)
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
#define malloc(x)    tempArena().alloc(x, 4)
#define relloc(x,s)  tempArena().realloc(x, s, 4)
#define free(x)      tempArena().free(x)

using namespace BugEngine;

%}

%token  TOK_ID
%token  VAL_STRING VAL_INTEGER VAL_FLOAT VAL_BOOLEAN

%token  KW_import KW_plugin

%type   <bValue>    VAL_BOOLEAN
%type   <iValue>    VAL_INTEGER
%type   <fValue>    VAL_FLOAT
%type   <sValue>    VAL_STRING
%type   <sValue>    TOK_ID
%type   <sValue>    fullname
%type   <value>     value

%union
{
    bool                bValue;
    i64                 iValue;
    double              fValue;
    char*               sValue;
    BugEngine::Value*   value;
}

%start  file

%%

file:
        imports
        decls
    ;

imports:
        imports import
    |
        /* empty */
    ;

import:
        decl_import
    |
        decl_plugin
    ;

decls:
        decls decl
    |
        /* empty */
    ;

decl:
        decl_object
    ;

decl_import:
        KW_import fullname';'
        {
            free($2);
        }
    ;

decl_plugin:
        KW_plugin TOK_ID
            {
                ((PackageBuilder::BuildContext*)param)->result->loadPlugin($2);
                free($2);
            }
        ';'
    ;

decl_object:
        editor_attributes
        TOK_ID '=' fullname
        {
            ref<PackageBuilder::Nodes::Object> object = ref<PackageBuilder::Nodes::Object>::create(packageBuilderArena(), ((PackageBuilder::BuildContext*)param)->result);
            ref<PackageBuilder::Nodes::Reference> reference = ref<PackageBuilder::Nodes::Reference>::create(packageBuilderArena(), ((PackageBuilder::BuildContext*)param)->result);
            reference->setName(inamespace($4));
            object->setMethod(reference);
            object->setName($2);
        }
        '('
            params
        ')'
        {
            free($2);
            free($4);
        }
    ;

editor_attributes:
        editor_attributes
        '[' attribute ']'
    |
        /* empty */
    ;

attribute:
        TOK_ID '=' value
        {
            free($1);
        }
    ;

params:
        params
        param
    |
        /* empty */
    ;

param:
        TOK_ID '=' value ';'
        {
            free($1);
        }
    ;

value:
        fullname
        {
            $$ = 0;
            free($1);
        }
    |
        VAL_BOOLEAN
        {
            $$ = 0;
        }
    |
        VAL_INTEGER
        {
            $$ = 0;
        }
    |
        VAL_FLOAT
        {
            $$ = 0;
        }
    |
        VAL_STRING
        {
            $$ = 0;
            free($1);
        }
    ;

fullname:
        TOK_ID { $$ = $1; }
    |
        TOK_ID '.' fullname { size_t s = strlen($1); s += strlen($3); s++; $$ = (char*)malloc(s+1); strcpy($$, $1); strcat($$, "."); strcat($$, $3); free($1); free($3); }
    ;

%%
