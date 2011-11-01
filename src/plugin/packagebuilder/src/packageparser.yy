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

#ifdef _MSC_VER
# include <malloc.h>
# pragma warning(push)
# pragma warning(disable:4065) // switch contains 'default' but no 'case' label
# pragma warning(disable:4244) // conversion from 'type1' to 'type2' : possible loss of data
# pragma warning(disable:4100) // param : unreferenced formal parameter
# pragma warning(disable:4702) // unreachable code
#endif

#define YYSTACK_USE_ALLOCA 1

struct Parameter
{
    Parameter*          next;
    BugEngine::istring  name;
    BugEngine::Value    value;
};

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
#define relloc(x,s)  BugEngine::tempArena().realloc(x, s, 4)
#define free(x)      BugEngine::tempArena().free(x)

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
        {
            free($2);
        }
    ;

decl_plugin:
        KW_plugin TOK_ID
            {
                BugEngine::istring i($2);
                BugEngine::Plugin<minitl::pointer> plugin (i, BugEngine::Plugin<minitl::pointer>::Preload);
                if (!plugin)
                {
                    yyerror((minitl::format<>("Unable to find plugin %s") | i).c_str());
                }
                else
                {
                    ((BugEngine::PackageBuilder::BuildContext*)param)->plugins.insert(std::make_pair(i, plugin));
                    ((BugEngine::PackageBuilder::BuildContext*)param)->imports.insert(std::make_pair(i, plugin.pluginNamespace()));
                }
                free($2);
            }
        ';'
    ;

decl_object:
        editor_attributes
        TOK_ID '=' fullname '('
            params
        ')'
        {
            BugEngine::inamespace n($4);
            minitl::hashmap<BugEngine::istring, const BugEngine::RTTI::ClassInfo*>::const_iterator it = ((BugEngine::PackageBuilder::BuildContext*)param)->imports.find(n[0]);
            if (it == ((BugEngine::PackageBuilder::BuildContext*)param)->imports.end())
            {
                yyerror((minitl::format<>("Could not find type %s") | n).c_str());
            }
            else if (!it->second)
            {
                yyerror((minitl::format<>("Plugin %s is empty") | n).c_str());
            }
            else
            {
                const BugEngine::RTTI::ClassInfo* ci = it->second;
                BugEngine::Value v(ci);
                for (size_t i = 1; i < n.size(); ++i)
                {
                    v = v[n[i]];
                    if (!v)
                    {
                        yyerror((minitl::format<>("Could not find object %s") | n).c_str());
                        break;
                    }
                }
                const BugEngine::RTTI::MethodInfo* mi = v.type().metaclass->call;
                if (!mi)
                {
                    yyerror((minitl::format<>("%s is not a type or callable object") | n).c_str());
                }
            }
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
