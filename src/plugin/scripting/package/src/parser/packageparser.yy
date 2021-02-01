/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */
%{
#include    <bugengine/plugin.scripting.package/stdafx.h>
#include    <buildcontext.hh>

#define yyparse be_package_parse
#define yylex   be_package_lex
#define yyerror be_package_error
#define yylval  be_package_lval
#define yychar  be_package_char
#define yydebug be_package_debug
#define yynerrs be_package_nerrs



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

static int yyerror(void* context, const char *msg)
{
    ((BugEngine::PackageBuilder::BuildContext*)context)->result->error(g_packageLine+1, msg);
    //be_error("%s at line %d (%d:%d)" | msg | (g_packageLine+1) | (g_packageColumnBefore+1) | (g_packageColumnAfter+1));
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
#define malloc(x)    BugEngine::Arena::temporary().alloc(x, 4)
#define realloc(x,s) BugEngine::Arena::temporary().realloc(x, s, 4)
#define free(x)      BugEngine::Arena::temporary().free(x)

using namespace BugEngine::PackageBuilder;
using namespace BugEngine::RTTI::AST;

%}

%token  TOK_ID TOK_value
%token  KW_import KW_plugin KW_as

%type   <sValue>            TOK_ID fullname
%type   <value>            TOK_value

%start  file
%parse-param { void*  param }

%destructor { free($$); }                   TOK_ID fullname
%destructor { $$->~ref(); free($$); }       TOK_value

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
        KW_import fullname ';'
        {
            free($2);
        }
    ;

decl_plugin:
        KW_plugin fullname ';'
            {
                ((BuildContext*)param)->result->loadPlugin($2, $2);
                free($2);
            }
    |
        KW_plugin fullname KW_as TOK_ID ';'
            {
                ((BuildContext*)param)->result->loadPlugin($2, $4);
                free($2);
                free($4);
            }
    ;

decl_object:
        editor_attributes
        TOK_ID '=' TOK_value
        {
            //(*$4)->setName($2);
            ((BuildContext*)param)->result->insertNode($2, *$4);
            $4->~ref();
            free($4);
            free($2);
        }
    ;

editor_attributes:
        editor_attributes
        '[' attribute ']'
    |
        /* empty */
    ;

attribute:
        TOK_ID '=' TOK_value
        {
            free($1);
            $3->~ref();
            free($3);
        }
    ;

fullname:
        TOK_ID
        {
            $$ = $1;
        }
    |
        TOK_ID '.' fullname
        {
            size_t s = strlen($1);
            s += strlen($3);
            s++;
            $$ = (char*)malloc(s+1);
            strcpy($$, $1);
            strcat($$, ".");
            strcat($$, $3);
            free($1);
            free($3);
        }
    ;
%%
