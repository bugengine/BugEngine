/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */
%{
#include    <rttiparse/stdafx.h>
#include    <parsecontext.hh>
#include    <rttiparse/object.hh>
#include    <nodes/reference.hh>
#include    <nodes/bool.hh>
#include    <nodes/integer.hh>
#include    <nodes/int2.hh>
#include    <nodes/int3.hh>
#include    <nodes/int4.hh>
#include    <nodes/float.hh>
#include    <nodes/float2.hh>
#include    <nodes/float3.hh>
#include    <nodes/float4.hh>
#include    <nodes/string.hh>
#include    <nodes/filename.hh>
#include    <nodes/array.hh>

#define yyparse be_value_parse
#define yylex   be_value_lex
#define yyerror be_value_error
#define yylval  be_value_lval
#define yychar  be_value_char
#define yydebug be_value_debug
#define yynerrs be_value_nerrs



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

static int yyerror(::BugEngine::RTTI::Parser::ParseContext* context, const char *msg)
{
    using namespace BugEngine::RTTI::Parser;
    context->errors.push_back(Error(context->location,
                              ErrorType("%s at line %d (%d:%d)")
                                      | msg
                                      | (context->location.line+1)
                                      | (context->location.columnStart+1)
                                      | (context->location.columnEnd+1)));
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

%}

%token VAL_BOOLEAN VAL_STRING VAL_INTEGER VAL_FLOAT VAL_FILENAME
%token TOK_ID TOK_EXPECT_OBJECT TOK_EXPECT_ANY
%token END 0 "end of file"
%parse-param { BugEngine::RTTI::Parser::ParseContext* context }

%start start


%type   <cValue>        '(' ')' '[' ']' ',' '='
%type   <bValue>        VAL_BOOLEAN
%type   <iValue>        VAL_INTEGER
%type   <fValue>        VAL_FLOAT
%type   <sValue>        VAL_STRING
%type   <sValue>        VAL_FILENAME
%type   <sValue>        TOK_ID
%type   <sValue>        fullname
%type   <value>         value
%type   <value>         object
%type   <param>         param
%type   <param_list>    param_list
%type   <value_list>    value_list

%destructor { free($$.value); }                         VAL_STRING VAL_FILENAME TOK_ID fullname
%destructor { $$.value->~Parameter(); free($$.value); } param
%destructor { $$.value->~ref(); free($$.value); }       object
%destructor { $$.value->~vector(); free($$.value); }    value_list param_list

%{
using namespace BugEngine::RTTI::Parser;
%}

%%

start:
        TOK_EXPECT_OBJECT
        object
        {
            context->result = *$2.value;
            $2.value->~ref();
            free($2.value);
        }
    |
        TOK_EXPECT_ANY
        value
        {
            context->result = *$2.value;
            $2.value->~ref();
            free($2.value);
        }
    ;

value:
        VAL_BOOLEAN
        {
            $$.value = reinterpret_cast< ref<Node>* >(malloc(sizeof(*$$.value)));
            new ($$.value) ref<Node>(ref<Bool>::create(*context->arena,
                                                       $1.location,
                                                       $1.value));
            $$.location = $1.location;
        }
    |
        VAL_STRING
        {
            $$.value = reinterpret_cast< ref<Node>* >(malloc(sizeof(*$$.value)));
            new ($$.value) ref<Node>(ref<String>::create(*context->arena,
                                                         $1.location,
                                                         $1.value));
            $$.location = $1.location;
            free($1.value);
        }
    |
        VAL_INTEGER
        {
            $$.value = reinterpret_cast< ref<Node>* >(malloc(sizeof(*$$.value)));
            new ($$.value) ref<Node>(ref<Integer>::create(*context->arena,
                                                          $1.location,
                                                          $1.value));
            $$.location = $1.location;
        }
    |
        '(' VAL_INTEGER ',' VAL_INTEGER ')'
        {
            $$.value = reinterpret_cast< ref<Node>* >(malloc(sizeof(*$$.value)));
            new ($$.value) ref<Node>(ref<Int2>::create(*context->arena,
                                                       $2.location,
                                                       make_bigint2($2.value, $4.value)));
            $$.location = $1.location;
        }
    |
        '(' VAL_INTEGER ',' VAL_INTEGER ',' VAL_INTEGER')'
        {
            $$.value = reinterpret_cast< ref<Node>* >(malloc(sizeof(*$$.value)));
            new ($$.value) ref<Node>(ref<Int3>::create(*context->arena,
                                                       $2.location,
                                                       make_bigint3($2.value, $4.value, $6.value)));
            $$.location = $1.location;
        }
    |
        '(' VAL_INTEGER ',' VAL_INTEGER ',' VAL_INTEGER ',' VAL_INTEGER ')'
        {
            $$.value = reinterpret_cast< ref<Node>* >(malloc(sizeof(*$$.value)));
            new ($$.value) ref<Node>(ref<Int4>::create(*context->arena,
                                                       $2.location,
                                                       make_bigint4($2.value, $4.value,
                                                                    $6.value, $8.value)));
            $$.location = $1.location;
        }
    |
        VAL_FLOAT
        {
            $$.value = reinterpret_cast< ref<Node>* >(malloc(sizeof(*$$.value)));
            new ($$.value) ref<Node>(ref<Float>::create(*context->arena,
                                                        $1.location,
                                                        $1.value));
            $$.location = $1.location;
        }
    |
        '(' VAL_FLOAT ',' VAL_FLOAT ')'
        {
            $$.value = reinterpret_cast< ref<Node>* >(malloc(sizeof(*$$.value)));
            new ($$.value) ref<Node>(ref<Float2>::create(*context->arena,
                                                         $1.location,
                                                         make_float2($2.value, $4.value)));
            $$.location = $1.location;
        }
    |
        '(' VAL_FLOAT ',' VAL_FLOAT ',' VAL_FLOAT')'
        {
            $$.value = reinterpret_cast< ref<Node>* >(malloc(sizeof(*$$.value)));
            new ($$.value) ref<Node>(ref<Float3>::create(*context->arena,
                                                         $1.location,
                                                         make_float3($2.value, $4.value, $6.value)));
            $$.location = $1.location;
        }
    |
        '(' VAL_FLOAT ',' VAL_FLOAT ',' VAL_FLOAT ',' VAL_FLOAT ')'
        {
            $$.value = reinterpret_cast< ref<Node>* >(malloc(sizeof(*$$.value)));
            new ($$.value) ref<Node>(ref<Float4>::create(*context->arena,
                                                         $1.location,
                                                         make_float4($2.value, $4.value,
                                                                     $6.value, $8.value)));
            $$.location = $1.location;
        }
    |
        VAL_FILENAME
        {
            $$.value = reinterpret_cast< ref<Node>* >(malloc(sizeof(*$$.value)));
            new ($$.value) ref<Node>(ref<FileName>::create(*context->arena,
                                                           $1.location,
                                                           $1.value));
            $$.location = $1.location;
            free($1.value);
        }
    |
        fullname
        {
            $$.value = reinterpret_cast< ref<Node>* >(malloc(sizeof(*$$.value)));
            new ($$.value) ref<Node>(ref<Reference>::create(*context->arena,
                                                            $1.location,
                                                            $1.value));
            $$.location = $1.location;
            free($1.value);
        }
    |
        object
        {
            $$ = $1;
        }
    |
        '[' value_list ']'
        {
            $$.value = reinterpret_cast< ref<Node>* >(malloc(sizeof(*$$.value)));
            new ($$.value) ref<Node>(ref<Array>::create(*context->arena,
                                                        $2.location,
                                                        *$2.value));
            $$.location = $2.location;
            $2.value->~vector();
            free($2.value);
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
            size_t s = strlen($1.value);
            s += strlen($3.value);
            s++;
            $$.value = (char*)malloc(s+1);
            strcpy($$.value, $1.value);
            strcat($$.value, ".");
            strcat($$.value, $3.value);
            free($1.value);
            free($3.value);
        }
    ;

object:
        fullname
        '('
            param_list
        ')'
        {
            if ($3.value->size() >= 2)
            {
                for (minitl::vector<Parameter>::iterator it = $3.value->begin();
                     it != $3.value->end() - 1;
                     ++it)
                {
                    for (minitl::vector<Parameter>::iterator it2 = it + 1;
                         it2 != $3.value->end();
                         /*nothing*/)
                    {
                        if (it->name == it2->name)
                        {
                            context->errors.push_back(Error(it2->location,
                                                            ErrorType("attribute %s specified several times")
                                                          | it->name));
                            context->errors.push_back(Error(it->location,
                                                            ErrorType("  first defined here")));
                            it2 = $3.value->erase(it2);
                        }
                        else
                        {
                            ++it2;
                        }
                    }
                }
            }
            ref<Reference> name = ref<Reference>::create(*context->arena,
                                                         $1.location,
                                                         BugEngine::inamespace($1.value));
            $$.value = reinterpret_cast< ref<Node>* >(malloc(sizeof(*$$.value)));
            new ($$.value) ref<Node>(ref<Object>::create(*context->arena,
                                                         $2.location,
                                                         name,
                                                         *$3.value));
            $3.value->~vector();
            free($3.value);
            free($1.value);
        }
    ;

value_list:
        /* empty */
        {
            $$.value = reinterpret_cast< minitl::vector< ref<Node> >* >(malloc(sizeof(*$$.value)));
            new ($$.value) minitl::vector< ref<Node> >(*context->arena);
        }
    |
        value
        {
            $$.value = reinterpret_cast< minitl::vector< ref<Node> >* >(malloc(sizeof(*$$.value)));
            new ($$.value) minitl::vector< ref<Node> >(*context->arena);
            $$.value->push_back(*$1.value);
            $1.value->~ref();
            free($1.value);
        }
    |
        value_list ',' value
        {
            $$.value = $1.value;
            $$.value->push_back(*$3.value);
            $3.value->~ref();
            free($3.value);
        }
    ;

param_list:
        /* empty */
        {
            $$.value = reinterpret_cast< minitl::vector<Parameter>* >(malloc(sizeof(*$$.value)));
            new ($$.value) minitl::vector<Parameter>(*context->arena);
        }
    |
        param_list
        param
        {
            $$.location = $2.location;
            $$.value = $1.value;
            $$.value->push_back(*$2.value);
            $2.value->~Parameter();
            free($2.value);
        }
    ;

param:
        TOK_ID '=' value ';'
        {
            $$.location = $1.location;
            $$.value = reinterpret_cast<Parameter*>(malloc(sizeof(*$$.value)));
            new ($$.value) Parameter;
            $$.value->location = $1.location;
            $$.value->name = BugEngine::istring($1.value);
            $$.value->value = *$3.value;
            $3.value->~ref();
            free($3.value);
            free($1.value);
        }
    ;

%%
