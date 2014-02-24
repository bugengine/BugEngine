/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */
%{
#include    <package/stdafx.h>
#include    <buildcontext.hh>
#include    <package/nodes/package.hh>
#include    <package/nodes/object.hh>
#include    <package/nodes/entity.hh>
#include    <package/nodes/component.hh>
#include    <package/nodes/reference.hh>
#include    <package/nodes/parameter.hh>
#include    <package/nodes/value.hh>
#include    <package/nodes/zip.hh>

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

static int yyerror(void*, const char *msg)
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
#define malloc(x)    BugEngine::Arena::temporary().alloc(x, 4)
#define realloc(x,s) BugEngine::Arena::temporary().realloc(x, s, 4)
#define free(x)      BugEngine::Arena::temporary().free(x)

using namespace BugEngine::PackageBuilder;
using namespace BugEngine::PackageBuilder::Nodes;


ref<Instance> s_currentInstance;
ref<Instance> s_previousInstance;
ref<ZipValue> s_currentZip;
ref<Entity> s_currentEntity;

%}

%token  TOK_ID
%token  VAL_STRING VAL_INTEGER VAL_FLOAT VAL_BOOLEAN VAL_FILENAME

%token  KW_import KW_plugin KW_namespace KW_zip KW_notify KW_as

%type   <bValue>        VAL_BOOLEAN
%type   <iValue>        VAL_INTEGER
%type   <fValue>        VAL_FLOAT
%type   <sValue>        VAL_STRING
%type   <sValue>        VAL_FILENAME
%type   <sValue>        TOK_ID
%type   <sValue>        fullname
%type   <value>         value value_zip
%type   <value_array>   value_array

%start  file
%parse-param { void*  param }

%destructor { free($$); }                   VAL_STRING VAL_FILENAME TOK_ID fullname
%destructor { $$->~ref(); free($$); }       value value_zip
%destructor { $$->~vector(); free($$); }    value_array

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
        KW_plugin fullname
            {
                ((BuildContext*)param)->result->loadPlugin($2, $2);
                free($2);
            }
	|
		KW_plugin fullname KW_as TOK_ID
            {
                ((BuildContext*)param)->result->loadPlugin($2, $4);
                free($2);
            }
        ';'
    ;

decl_object:
        {
            ref<Object> newObject = ref<Object>::create(BugEngine::Arena::packageBuilder(), ((BuildContext*)param)->result);
            s_currentInstance = newObject;
            ((BuildContext*)param)->result->insertNode(newObject);
        }
        editor_attributes
        TOK_ID '=' fullname
        {
            ref<Nodes::Reference> reference = ref<Reference>::create(BugEngine::Arena::packageBuilder(), ((BuildContext*)param)->result);
            reference->setName(BugEngine::inamespace($5));
            s_currentInstance->setName($3);
            be_checked_cast<Object>(s_currentInstance)->setMethod(reference);
        }
        '('
            params
        ')'
        {
            s_currentInstance.clear();
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
            $3->~ref();
            free($3);
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
            ref<Parameter> parameter = ref<Parameter>::create(BugEngine::Arena::packageBuilder(), BugEngine::istring($1), *$3);
            s_currentInstance->addParameter(parameter);
            $3->~ref();
            free($1);
            free($3);
        }
    |
        TOK_ID ':'  TOK_ID '=' value ';'
        {
            ref<Parameter> parameter = ref<Parameter>::create(BugEngine::Arena::packageBuilder(), BugEngine::istring($3), *$5);
            s_currentInstance->addParameter(parameter);
            $5->~ref();
            free($1);
            free($3);
            free($5);
        }
    ;

value:
        fullname
        {
            $$ = (ref<Value>*)malloc(sizeof(*$$));
            ref<Reference> reference = ref<Reference>::create(BugEngine::Arena::packageBuilder(), ((BuildContext*)param)->result);
            reference->setName(BugEngine::inamespace($1));
            new ($$) ref<Value>(ref<ReferenceValue>::create(BugEngine::Arena::packageBuilder(), reference));
            free($1);
        }
    |
        VAL_BOOLEAN
        {
            $$ = (ref<Value>*)malloc(sizeof(*$$));
            new ($$) ref<Value>(ref<BoolValue>::create(BugEngine::Arena::packageBuilder(), $1));
        }
    |
        VAL_INTEGER
        {
            $$ = (ref<Value>*)malloc(sizeof(*$$));
            new ($$) ref<Value>(ref<IntValue>::create(BugEngine::Arena::packageBuilder(), $1));
        }
    |
        VAL_FLOAT
        {
            $$ = (ref<Value>*)malloc(sizeof(*$$));
            new ($$) ref<Value>(ref<FloatValue>::create(BugEngine::Arena::packageBuilder(), $1));
        }
    |
        VAL_STRING
        {
            $$ = (ref<Value>*)malloc(sizeof(*$$));
            new ($$) ref<Value>(ref<StringValue>::create(BugEngine::Arena::packageBuilder(), $1));
            free($1);
        }
    |
        VAL_FILENAME
        {
            $$ = (ref<Value>*)malloc(sizeof(*$$));
            new ($$) ref<Value>(ref<FileValue>::create(BugEngine::Arena::packageBuilder(), ((BuildContext*)param)->folder, $1));
            free($1);
        }
    |
        '[' value_array ']'
        {
            $$ = (ref<Value>*)malloc(sizeof(*$$));
            new ($$) ref<Value>(ref<ArrayValue>::create(BugEngine::Arena::packageBuilder(), *$2));
            $2->~vector();
            free($2);
        }
    |
        value_zip
        {
            $$ = $1;
        }
    ;

value_array:
        /*empty*/
        {
            $$ = (minitl::vector< ref<Value> >*)malloc(sizeof(*$$));
            new ($$) minitl::vector< ref<Value> >(BugEngine::Arena::packageBuilder());
        }
    |
        value
        {
            $$ = (minitl::vector< ref<Value> >*)malloc(sizeof(*$$));
            new ($$) minitl::vector< ref<Value> >(BugEngine::Arena::packageBuilder());
            $$->push_back(*$1);
            $1->~ref();
            free($1);
        }
    |
        value ',' value_array
        {
            $$ = $3;
            $$->push_back(*$1);
            $1->~ref();
            free($1);
        }
    ;

value_zip:
        KW_zip
        {
            $<value>$ = (ref<Value>*)malloc(sizeof(*$<value>$));
            s_currentZip = ref<ZipValue>::create(BugEngine::Arena::packageBuilder(), ((BuildContext*)param)->result);
            new ($<value>$) ref<Value>(s_currentZip);
        }
        '(' zip_array ')'
        {
            $$ = $<value>2;
            s_currentZip = ref<ZipValue>();
        }
    ;

zip_array:
        /* empty */
    |
        zip_value ';' zip_array
    ;

zip_value:
        TOK_ID '='
        {
            s_currentEntity = ref<Entity>::create(BugEngine::Arena::packageBuilder());
            s_currentZip->addEntity(s_currentEntity);
            free($1);
        }
        '(' component_array ')'
        {
            s_currentEntity = ref<Entity>();
        }
    |
        TOK_ID '.' TOK_ID '.' TOK_ID KW_notify TOK_ID '.' TOK_ID '.' TOK_ID
        {
            free($1);
            free($3);
            free($5);
            free($7);
            free($9);
            free($11);
        }
    ;

component_array:
        /* empty */
    |
        decl_component
    |
        component_array ',' decl_component
    ;

decl_component:
        fullname
        {
            ref<Component> component = minitl::ref<Component>::create(BugEngine::Arena::packageBuilder(), ((BuildContext*)param)->result);
            s_previousInstance = s_currentInstance;
            s_currentInstance = component;
            s_currentEntity->addComponent(component);
            free($1);
        }
        '(' params ')'
        {
            s_currentInstance = s_previousInstance;
            s_previousInstance = ref<Instance>();
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
