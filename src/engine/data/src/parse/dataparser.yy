%{

#define YYPARSE_PARAM param
#include    <data/stdafx.h>

#ifdef _MSC_VER
# include <malloc.h>
# pragma warning(push)
# pragma warning(disable:4065) // switch contains 'default' but no 'case' label
# pragma warning(disable:4244) // conversion from 'type1' to 'type2' : possible loss of data
# pragma warning(disable:4100) // param : unreferenced formal parameter
# pragma warning(disable:4702) // unreachable code
#endif
#ifdef _WIN32
static inline int isatty(int)
{
    return 0;
}
#endif

#include <cstdio>

#include <parse/database.hh>
#include <parse/node.hh>
#include <parse/reference.hh>
#include <parse/type.hh>

using namespace BugEngine::Data::Parse;

#define YYSTACK_USE_ALLOCA 1

static Database*   g_context = 0;
       int         g_line = 0;
       int         g_columnbefore = 0;
       int         g_columnafter = 0;

static int yyerror(const char *msg)
{
    fprintf(stderr, "line %d (%d-%d) : %s\n", g_line, g_columnbefore, g_columnafter, msg);
    return 0;
}
extern int yylex();

%}


%union
{
    bool                    boolval;
    i64                     intval;
    double                  floatval;
    char*                   strval;
    Node*                   node;
    Type*                   type;
    Database::Visibility    visibility;
}



%token  KW_class
%token  TOK_bparallel TOK_eparallel TOK_bsequential TOK_esequential TOK_bsimultaneous TOK_esimultaneous
%token  KW_any KW_bool KW_int KW_float KW_string KW_vector KW_map KW_typeof KW_namespace
%token  KW_private KW_protected KW_package KW_public KW_export
%token  VAL_id VAL_bool VAL_int VAL_float VAL_string

%type   <boolval>       VAL_bool
%type   <intval>        VAL_int
%type   <floatval>      VAL_float
%type   <strval>        VAL_string
%type   <strval>        VAL_id

%type   <visibility>    visibility
%type   <type>          type

%left   '+' '-'
%left   '*' '/'
%left   '.'

%start  file

%%

file:
        {
            be_assert(g_context == 0, "context was already set");
            g_context = reinterpret_cast<Database*>(param);
            g_line = 1;
            g_columnbefore = 0;
            g_columnafter = 1;
        }
        decls
        {
            g_context = 0;
        }
    ;
    
visibility:
        KW_private      { $$ = Database::Private; }
    |
        KW_protected    { $$ = Database::Protected; }
    |
        KW_package      { $$ = Database::Package; }
    |
        KW_public       { $$ = Database::Public; }
    |
        KW_export       { $$ = Database::Export; }
    |
        /**/            { $$ = Database::Package; }
    ;
        
    
decls:
        decls
        decl
    |
        decls
        error
    |
        /*empty*/
    ;

decl:
        namespace
    |
        visibility function
        {
            //g_context->add($2->name(), $1, $2);
        }
    |
        visibility VAL_id '=' atom
        {
        }
    ;
    
namespace:
        KW_namespace VAL_id
        '{'         { g_context->push($2); }
            decls
        '}'         { g_context->pop($2); }
    ;
    
    
function:
        type VAL_id '(' parameters ')'
        compositeffect
    ;

type:
        KW_any                          { $$ = new Type::Any; }
    |
        KW_bool                         { $$ = new Type::Bool; }
    |
        KW_int                          { $$ = new Type::Int; }
    |
        KW_float                        { $$ = new Type::Float; }
    |
        KW_string                       { $$ = new Type::String; }
    |
        KW_vector '<' type '>'          { $$ = new Type::Vector($3); }
    |
        KW_map '<' type ',' type '>'    { $$ = new Type::Map($3,$5); }
    ;
    
parameters:
        /*empty*/
    ;

compositeffect:
        '{'
        effectlist
        '}'
    |
        TOK_bparallel
        effectlist
        TOK_eparallel
    |
        TOK_bsimultaneous
        effectlist
        TOK_esimultaneous
    ;
    
effectlist:
        effectlist
        effect      
    |
        /*empty*/
    ;

effect:
        ';'
    ;
    
atom:
        VAL_bool VAL_int VAL_float VAL_string
    ;
%%


#ifdef _MSC_VER
# pragma warning(pop)
#endif

