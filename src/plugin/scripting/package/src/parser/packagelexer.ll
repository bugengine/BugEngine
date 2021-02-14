/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */
%{
#include    <bugengine/plugin.scripting.package/stdafx.h>
#include    <ctype.h>
#include    <buildcontext.hh>
#include    <bugengine/rtti-parse/valueparse.hh>

#define yylval  be_package_lval
#include "packageparser.hh"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4127 4244 4267 4996 4505)
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
#define relloc(x,s)  BugEngine::Arena::temporary().realloc(x, s, 4)
#define free(x)      BugEngine::Arena::temporary().free(x)
#ifdef strdup
# undef strdup
#endif
#define strdup(x)    be_strdup(x)

static char *be_strdup(const char *src)
{
    size_t x = strlen(src);
    char* result = (char*)malloc(x+1);
    strncpy(result, src, x+1);
    return result;
}

static void update (int num)
{
    g_packageOffset += num;
    g_packageColumnBefore = g_packageColumnAfter;
    g_packageColumnAfter += num;
}
static void newline()
{
    g_packageOffset ++;
    g_packageLine++;
    g_packageColumnBefore = 0;
    g_packageColumnAfter = 1;
}

extern "C" int be_package_wrap()
{
    return 1;
}

#define YY_INPUT(buf,result,max_size)                               \
        {                                                           \
            result = max_size > g_buffer->count()-g_bufferPosition  \
                ? g_buffer->count()-g_bufferPosition                \
                : max_size;                                         \
            memcpy(buf, g_buffer->data()+g_bufferPosition, result); \
            g_bufferPosition += result;                             \
        }



%}

%option prefix="be_package_"
%option never-interactive
%option nounput

%x RTTIPARSE

%%

<INITIAL>{
    import {
         update(be_package_leng);
         return KW_import;
    }

    load {
        update(be_package_leng);
        return KW_plugin;
    }

    as {
        update(be_package_leng);
        return KW_as;
    }

    [A-Za-z_][0-9A-Za-z_<>]* {
        update(be_package_leng);
        yylval.id = be_strdup(be_package_text);
        return TOK_ID;
    }

    "\n" {
        (void)&yyinput;
        newline();
    }

    [ \r\t]+ {
        update(be_package_leng);
    }

    \#[^\n]*\n {
        update(be_package_leng);
    }

    "=" {
        update(be_package_leng);
        
        BEGIN(RTTIPARSE);
        yylval.offset.line = g_packageLine;
        yylval.offset.column = g_packageColumnAfter;
        yylval.offset.start = g_packageOffset;
        return *be_package_text;
    }

    . {
        update(be_package_leng);
        return *be_package_text;
    }
}

<RTTIPARSE>{
    ";" {
        update(be_package_leng);
        if (g_packageObjectNestedLevel == 0)
        {
            BEGIN(INITIAL);
            yylval.offset.end = g_packageOffset-1;
            return TOK_value;
        }
    }

    "(" {
        update(be_package_leng);
        ++g_packageObjectNestedLevel;
    }

    ")" {
        update(be_package_leng);
        --g_packageObjectNestedLevel;
    }

    "\n" {
        (void)&yyinput;
        newline();
    }

    . {
        update(be_package_leng);
    }
}


%%

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#if (YY_FLEX_MINOR_VERSION < 5) || (YY_FLEX_MINOR_VERSION == 5 && YY_FLEX_SUBMINOR_VERSION < 5)
int be_package_lex_destroy()
{
    yy_delete_buffer(yy_current_buffer);
    return 0;
}
#endif
