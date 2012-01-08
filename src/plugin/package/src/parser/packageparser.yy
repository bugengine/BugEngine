%{
#include    <package/stdafx.h>
#include    <buildcontext.hh>
#include    <package/nodes/package.hh>
#include    <package/nodes/object.hh>
#include    <package/nodes/reference.hh>
#include    <package/nodes/parameter.hh>
#include    <package/nodes/value.hh>

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
#define malloc(x)    BugEngine::tempArena().alloc(x, 4)
#define realloc(x,s) BugEngine::tempArena().realloc(x, s, 4)
#define free(x)      BugEngine::tempArena().free(x)

using namespace BugEngine::PackageBuilder;
using namespace BugEngine::PackageBuilder::Nodes;


ref<Object> s_currentObject;

%}

%token  TOK_ID
%token  VAL_STRING VAL_INTEGER VAL_FLOAT VAL_BOOLEAN

%token  KW_import KW_plugin KW_namespace

%type   <bValue>    VAL_BOOLEAN
%type   <iValue>    VAL_INTEGER
%type   <fValue>    VAL_FLOAT
%type   <sValue>    VAL_STRING
%type   <sValue>    TOK_ID
%type   <sValue>    fullname
%type   <value>     value

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
                ((BuildContext*)param)->result->loadPlugin($2);
                free($2);
            }
        ';'
    ;

decl_object:
        {
            s_currentObject = ref<Object>::create(packageBuilderArena(), ((BuildContext*)param)->result);
            ((BuildContext*)param)->result->insertNode(s_currentObject);
        }
        editor_attributes
        TOK_ID '=' fullname
        {
            ref<Nodes::Reference> reference = ref<Reference>::create(packageBuilderArena(), ((BuildContext*)param)->result);
            reference->setName(BugEngine::inamespace($5));
            s_currentObject->setMethod(reference);
            s_currentObject->setName($3);
        }
        '('
            params
        ')'
        {
            s_currentObject.clear();
            free($3);
            free($5);
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
            ref<Parameter> parameter = ref<Parameter>::create(packageBuilderArena(), BugEngine::istring($1), *$3);
            s_currentObject->addParameter(parameter);
            $3->~ref();
            free($1);
            free($3);
        }
    ;

value:
        fullname
        {
            $$ = (ref<Value>*)malloc(sizeof(*$$));
            ref<Reference> reference = ref<Reference>::create(packageBuilderArena(), ((BuildContext*)param)->result);
            reference->setName(BugEngine::inamespace($1));
            new ($$) ref<Value>(ref<ReferenceValue>::create(packageBuilderArena(), reference));
            free($1);
        }
    |
        VAL_BOOLEAN
        {
            $$ = (ref<Value>*)malloc(sizeof(*$$));
            new ($$) ref<Value>(ref<BoolValue>::create(packageBuilderArena(), $1));
        }
    |
        VAL_INTEGER
        {
            $$ = (ref<Value>*)malloc(sizeof(*$$));
            new ($$) ref<Value>(ref<IntValue>::create(packageBuilderArena(), $1));
        }
    |
        VAL_FLOAT
        {
            $$ = (ref<Value>*)malloc(sizeof(*$$));
            new ($$) ref<Value>(ref<FloatValue>::create(packageBuilderArena(), $1));
        }
    |
        VAL_STRING
        {
            $$ = (ref<Value>*)malloc(sizeof(*$$));
            new ($$) ref<Value>(ref<StringValue>::create(packageBuilderArena(), $1));
            free($1);
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
