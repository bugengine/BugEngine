%{

#define yyparse be_settings_parse
#define yylex   be_settings_lex
#define yyerror be_settings_error
#define yylval  be_settings_lval
#define yychar  be_settings_char
#define yydebug be_settings_debug
#define yynerrs be_settings_nerrs


#define __attribute__(x)
#define YYPARSE_PARAM param
#include    <core/stdafx.h>

#ifdef _MSC_VER
# include <malloc.h>
# pragma warning(push)
# pragma warning(disable:4065) // switch contains 'default' but no 'case' label
# pragma warning(disable:4244) // conversion from 'type1' to 'type2' : possible loss of data
# pragma warning(disable:4100) // param : unreferenced formal parameter
# pragma warning(disable:4702) // unreachable code
#endif

#define YYSTACK_USE_ALLOCA 1

static int yyerror(const char *msg)
{
    fprintf(stderr, "%s\n", msg);
    return 0;
}
extern int yylex();
int         g_settingsLine = 0;
int         g_settingsColumnBefore = 0;
int         g_settingsColumnAfter = 0;

%}


%start  file

%token  TOK_ID
%token  VAL_STRING VAL_INTEGER VAL_FLOAT VAL_BOOLEAN

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


%%

file:
        category_list
    ;

category_list:
        category
        category_list
    |
        /* empty */
    ;

category:
        '[' TOK_ID ']'
        { }
        variable_list
    ;
    
variable_list:
        variable
        variable_list
    |
        /* empty */
    ;

variable:
        TOK_ID '=' VAL_INTEGER
        {
            minitl::format<> str = minitl::format<>("%s = %d\n") | $1 | $3;
        }
    |
        TOK_ID '=' VAL_BOOLEAN
        {
            minitl::format<> str = minitl::format<>("%s = %s\n") | $1 | ($3?"true":"false");
        }
    |
        TOK_ID '=' VAL_FLOAT
        {
            minitl::format<> str = minitl::format<>("%s = %s\n") | $1 | "float";
        }
    |
        TOK_ID '=' VAL_STRING
        {
            minitl::format<> str = minitl::format<>("%s = %s\n") | $1 | $3;
        }
    ;

%%

#ifdef _MSC_VER
# pragma warning(pop)
#endif
