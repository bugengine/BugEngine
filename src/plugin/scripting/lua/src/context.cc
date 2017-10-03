/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <stdafx.h>

#include    <context.hh>
#include    <runtime/plugin.hh>
#include    <runtime/object.hh>
#include    <runtime/resource.hh>


namespace BugEngine
{

namespace Arena
{
static minitl::Allocator& lua()
{
    return script();
}
}

namespace Lua
{

static ref<Logger> s_logger(Logger::instance("scripting.lua"));
static const raw<const RTTI::Class> s_voidClass = be_typeid<void>::klass();

static const char* s_metaTables[] = {
    "BugEngine.Object",
    "BugEngine.Resource",
    "BugEngine.ResourceManager",
    "BugEngine.Plugin"
};

static const luaL_Reg loadedlibs[] = {
  {"_G", luaopen_base},
  //{LUA_LOADLIBNAME, luaopen_package},
  {LUA_COLIBNAME, luaopen_coroutine},
  {LUA_TABLIBNAME, luaopen_table},
  //{LUA_IOLIBNAME, luaopen_io},
  //{LUA_OSLIBNAME, luaopen_os},
  {LUA_STRLIBNAME, luaopen_string},
  {LUA_BITLIBNAME, luaopen_bit32},
  {LUA_MATHLIBNAME, luaopen_math},
  {LUA_DBLIBNAME, luaopen_debug},
  {NULL, NULL}
};

void Context::typeError(lua_State* state, int narg, const char* expected, const char* got)
{
    Context::printStack(state);
    const char* msg = lua_pushfstring(state, "%s expected, got %s", expected, got);
    luaL_argerror(state, narg, msg);
}

void Context::checkArg(lua_State* state, int narg, int type)
{
    if (lua_type(state, narg) != type)
    {
        typeError(state, narg, lua_typename(state, type), luaL_typename(state, narg));
    }
}

void Context::checkArg(lua_State* state, int narg, const char *userDataType)
{
    if (lua_type(state, narg) != LUA_TUSERDATA)
    {
        typeError(state, narg, userDataType, luaL_typename(state, narg));
    }
    lua_getmetatable(state, narg);
    luaL_getmetatable(state, userDataType);
    if (!lua_rawequal(state, -1, -2))
    {
        lua_pop(state, 1);
        const char* typeName = luaL_typename(state, narg);
        for (u32 i = 0; i < sizeof(s_metaTables)/sizeof(s_metaTables[0]); ++i)
        {
            luaL_getmetatable(state, s_metaTables[i]);
            if (lua_rawequal(state, -1, -2))
            {
                typeName = s_metaTables[i];
            }
            lua_pop(state, 1);
        }
        lua_pop(state, 1);
        typeError(state, narg, userDataType, typeName);
    }
    lua_pop(state, 2);
}

void Context::checkArg(lua_State* state, int narg, const RTTI::Type& type)
{
    if (lua_type(state, narg) != LUA_TUSERDATA)
    {
        typeError(state, narg, type.name().c_str(), luaL_typename(state, narg));
    }
    lua_getmetatable(state, narg);
    luaL_getmetatable(state, "BugEngine.Object");
    if (!lua_rawequal(state, -1, -2))
    {
        lua_pop(state, 1);
        const char* typeName = luaL_typename(state, narg);
        for (u32 i = 0; i < sizeof(s_metaTables)/sizeof(s_metaTables[0]); ++i)
        {
            luaL_getmetatable(state, s_metaTables[i]);
            if (lua_rawequal(state, -1, -2))
            {
                typeName = s_metaTables[i];
            }
            lua_pop(state, 1);
        }
        lua_pop(state, 1);
        typeError(state, narg, type.name().c_str(), typeName);
    }
    lua_pop(state, 2);
    RTTI::Value* value = (RTTI::Value*)lua_touserdata(state, narg);
    if (!value->type().isA(type))
    {
        typeError(state, narg, type.name().c_str(), value->type().name().c_str());
    }
}

int Context::push(lua_State* state, const RTTI::Value& v)
{
    const RTTI::Type& t = v.type();
    if (t.metaclass == s_voidClass)
    {
        return 0;
    }
    else if (v.type().indirection >= RTTI::Type::RawPtr && v.as<const void* const>() == 0)
    {
        lua_pushnil(state);
        return 1;
    }
    else if (t.metaclass->type() == RTTI::ClassType_Number)
    {
        switch(t.metaclass->index())
        {
        case RTTI::ClassIndex_bool:
            lua_pushboolean(state, v.as<bool>());
            return 1;
        case RTTI::ClassIndex_u8:
            lua_pushnumber(state, v.as<u8>());
            return 1;
        case RTTI::ClassIndex_u16:
            lua_pushnumber(state, v.as<u16>());
            return 1;
        case RTTI::ClassIndex_u32:
            lua_pushnumber(state, v.as<u32>());
            return 1;
        case RTTI::ClassIndex_u64:
            lua_pushnumber(state, (lua_Number)v.as<u64>());
            return 1;
        case RTTI::ClassIndex_i8:
            lua_pushnumber(state, v.as<i8>());
            return 1;
        case RTTI::ClassIndex_i16:
            lua_pushnumber(state, v.as<i16>());
            return 1;
        case RTTI::ClassIndex_i32:
            lua_pushnumber(state, v.as<i32>());
            return 1;
        case RTTI::ClassIndex_i64:
            lua_pushnumber(state, (lua_Number)v.as<i64>());
            return 1;
        case RTTI::ClassIndex_float:
            lua_pushnumber(state, v.as<float>());
            return 1;
        case RTTI::ClassIndex_double:
            lua_pushnumber(state, v.as<double>());
            return 1;
        default:
            be_notreached();
            lua_pushnumber(state, 0);
            return 1;
        }
    }
    else
    {
        void* userdata = lua_newuserdata(state, sizeof (RTTI::Value));
        new(userdata) RTTI::Value(v);
        luaL_getmetatable(state, "BugEngine.Object");
        lua_setmetatable(state, -2);
        return 1;
    }
}

minitl::format<1024u> Context::tostring(lua_State* state, int element)
{
    int t = lua_type(state, element);
    switch (t)
    {
        case LUA_TSTRING:
            return minitl::format<1024u>("lua_string('%s')") | lua_tostring(state, element);
        case LUA_TBOOLEAN:
            return minitl::format<1024u>("lua_boolean(%s)") | lua_toboolean(state, element);
        case LUA_TNUMBER:
            return minitl::format<1024u>("lua_number(%g)") | lua_tonumber(state, element);
        case LUA_TUSERDATA:
        {
            lua_getmetatable(state, element);
            luaL_getmetatable(state, "BugEngine.Object");
            if (lua_rawequal(state, -1, -2))
            {
                lua_pop(state, 2);
                RTTI::Value* userdata = (RTTI::Value*)lua_touserdata(state, element);
                const char* constness = (userdata->type().constness == RTTI::Type::Const) ? "const " : "mutable ";
                const char* reference;
                const char* closing;
                switch(userdata->type().indirection)
                {
                    case RTTI::Type::RefPtr:
                        reference = "ref<";
                        closing = ">";
                        break;
                    case RTTI::Type::WeakPtr:
                        reference = "weak<";
                        closing = ">";
                        break;
                    case RTTI::Type::RawPtr:
                        reference = "raw<";
                        closing = ">";
                        break;
                    case RTTI::Type::Value:
                        reference = "";
                        constness = "";
                        closing = "";
                        break;
                    default:
                        be_notreached();
                        reference = "??? <";
                        closing=">";
                        break;
                }
                const char* access = (userdata->type().access == RTTI::Type::Const) ? "const " : "";
                return minitl::format<1024u>("%s%s%s%s%s[@0x%p]") | constness | reference | access | userdata->type().metaclass->name.c_str() | closing | userdata;
            }
            lua_pop(state, 1);
            luaL_getmetatable(state, "BugEngine.Plugin");
            if (lua_rawequal(state, -1, -2))
            {
                lua_pop(state, 2);
                Plugin::Plugin<void>* userdata = (Plugin::Plugin<void>*)lua_touserdata(state, element);
                return minitl::format<1024u>("BugEngine.Plugin[%s]") | userdata->name();
            }
            lua_pop(state, 1);
            luaL_getmetatable(state, "BugEngine.ResourceManager");
            if (lua_rawequal(state, -1, -2))
            {
                lua_pop(state, 2);
                return minitl::format<1024u>("BugEngine.ResourceManager");
            }
            lua_pop(state, 1);
            luaL_getmetatable(state, "BugEngine.Resource");
            if (lua_rawequal(state, -1, -2))
            {
                lua_pop(state, 2);
                return minitl::format<1024u>("BugEngine.Resource");
            }
            else
            {
                lua_pop(state, 2);
                return minitl::format<1024u>("lua_userdata[@0x%p]") | lua_touserdata(state, element);
            }
        }
        default:
            return minitl::format<1024u>("%s") | lua_typename(state, t);
    }
}

void Context::printStack(lua_State* state)
{
    int i;
    int top = lua_gettop(state);

    be_debug("total in stack %d\n" | top);

    for (i = 1; i <= top; i++)
    {
        be_debug(" %d: %s" | (top-i+1) | tostring(state, -i).c_str());
    }
}

extern "C" int luaPrint(lua_State *L)
{
    int n = lua_gettop(L); /* number of arguments */
    int i;
    lua_getglobal(L, "tostring");
    for (i = 1; i <= n; i++)
    {
        const char *s;
        lua_pushvalue(L, -1); /* function to be called */
        lua_pushvalue(L, i); /* value to print */
        lua_call(L, 1, 1);
        s = lua_tostring(L, -1); /* get result */
        if (s == NULL)
            return luaL_error(L, LUA_QL("tostring") " must return a string to "
                              LUA_QL("print"));
        lua_Debug ar;
        if (lua_getstack(L, 1, &ar))
        {
            lua_getinfo(L, "Snl", &ar);
            s_logger->log(logInfo, ar.source, ar.currentline, s);
        }
        else
        {
            s_logger->log(logInfo, BE_FILE, BE_LINE, s);
        }
        lua_pop(L, 1); /* pop result */
    }
    return 0;
}

extern "C" int luaPlugin(lua_State* state)
{
    int n = lua_gettop(state); /* number of arguments */
    if (n != 1)
    {
        return luaL_error(state, "plugin method expects one argument; got %d", n);
    }
    const char *pluginName = lua_tostring(state, -1);
    void* userdata = lua_newuserdata(state, sizeof (Plugin::Plugin<void>));
    new(userdata) Plugin::Plugin<void>(inamespace(pluginName), Plugin::Plugin<void>::Preload);
    luaL_getmetatable(state, "BugEngine.Plugin");
    lua_setmetatable(state, -2);
    return 1;
}

extern "C" int luaGet(lua_State* state)
{
    int n = lua_gettop(state); /* number of arguments */
    if (n != 2)
    {
        return luaL_error(state, "getattr method expects two arguments; got %d", n);
    }
    Context::checkArg(state, 1, "BugEngine.Object");
    Context::checkArg(state, 2, LUA_TSTRING);

    RTTI::Value* userdata = (RTTI::Value*)lua_touserdata(state, -2);
    const char *name = lua_tostring(state, -1);
    RTTI::Value v = (*userdata)[name];
    if (!v)
    {
        lua_pushnil(state);
        return 1;
    }
    Context::push(state, v);
    return 1;
}

extern "C" int luaGetType(lua_State* state)
{
    int n = lua_gettop(state); /* number of arguments */
    if (n != 1)
    {
        return luaL_error(state, "getattr method expects one argument; got %d", n);
    }
    Context::checkArg(state, 1, "BugEngine.Object");

    RTTI::Value* userdata = (RTTI::Value*)lua_touserdata(state, -2);
    Context::push(state, RTTI::Value(userdata->type()));
    return 1;
}

static const luaL_Reg base_funcs[] = {
    {"print", luaPrint},
    {"plugin", luaPlugin},
    {"getattr", luaGet},
    {"gettype", luaGetType},
    {NULL, NULL}
};

void* Context::luaAlloc(void* /*ud*/, void* ptr, size_t osize, size_t nsize)
{
    if (nsize)
    {
        if (osize)
        {
            return Arena::lua().realloc(ptr, nsize, 16);
        }
        else
        {
            return Arena::lua().alloc(nsize, 16);
        }
    }
    else
    {
        Arena::lua().free(ptr);
        return 0;
    }
}

minitl::format<1024u> Context::getCallInfo(lua_State *state)
{
    lua_Debug ar0, ar1;
    const char* source = "unknown source";
    const char* name = "unknown method";
    int line = -1;
    if (lua_getstack(state, 0, &ar0) && lua_getinfo(state, "n", &ar0))
    {
        name = ar0.name;
    }
    if (lua_getstack(state, 1, &ar1) && lua_getinfo(state, "Sl", &ar1))
    {
        source = ar1.source;
        line = ar1.currentline;
    }
    return minitl::format<1024u>("%s:%d (%s)") | source | line | name;
}

Context::Context(const Plugin::Context& context)
    :   ScriptEngine<LuaScript>(Arena::lua(), context.resourceManager)
    ,   m_state(lua_newstate(&Context::luaAlloc, 0))
{
    for (const luaL_Reg* lib = loadedlibs; lib->func; lib++)
    {
        luaL_requiref(m_state, lib->name, lib->func, 1);
        lua_pop(m_state, 1);
    }

    luaL_newmetatable(m_state, "BugEngine.Object");
    lua_pushstring(m_state, "__index");
    lua_pushvalue(m_state, -2);
    lua_settable(m_state, -3);
    luaL_setfuncs(m_state, s_valueMetaTable, 0);

    luaL_newmetatable(m_state, "BugEngine.Plugin");
    lua_pushstring(m_state, "__index");
    lua_pushvalue(m_state, -2);
    lua_settable(m_state, -3);
    luaL_setfuncs(m_state, s_pluginMetaTable, 0);

    luaL_newmetatable(m_state, "BugEngine.Resource");
    lua_pushstring(m_state, "__index");
    lua_pushvalue(m_state, -2);
    lua_settable(m_state, -3);
    luaL_setfuncs(m_state, s_resourceMetaTable, 0);

    luaL_newmetatable(m_state, "BugEngine.ResourceManager");
    lua_pushstring(m_state, "__index");
    lua_pushvalue(m_state, -2);
    lua_settable(m_state, -3);
    luaL_setfuncs(m_state, s_resourceLoaderMetaTable, 0);

    void* udata = lua_newuserdata(m_state, sizeof(weak<Resource::ResourceManager>));
    weak<Resource::ResourceManager>* manager = (weak<Resource::ResourceManager>*)udata;
    new (manager) weak<Resource::ResourceManager>(context.resourceManager);
    luaL_getmetatable(m_state, "BugEngine.ResourceManager");
    lua_setmetatable(m_state, -2);
    lua_setglobal(m_state, "resources");

    push(m_state, RTTI::Value(be_bugengine_Namespace()));
    lua_setglobal(m_state, "BugEngine");
    for (const luaL_Reg* method = base_funcs; method->func != 0; ++method)
    {
        lua_pushcfunction(m_state, method->func);
        lua_setglobal(m_state, method->name);
    }
}

Context::~Context()
{
    lua_close(m_state);
}

void Context::unload(Resource::Resource& /*handle*/)
{
}

void Context::runBuffer(weak<const LuaScript> script, Resource::Resource& /*resource*/, const minitl::Allocator::Block<u8>& block)
{
    int result;
    ifilename filename = script->getScriptName();
    result = luaL_loadbuffer(m_state,
                             (const char *) block.data(),
                             be_checked_numcast<size_t >(block.count()-1),
                             filename.str().name);
    if (result == 0)
    {
        result = lua_pcall(m_state, 0, LUA_MULTRET, 0);
    }
    be_assert(result == 0, lua_tostring(m_state, -1));
}

void Context::reloadBuffer(weak<const LuaScript> script, Resource::Resource& /*resource*/, const minitl::Allocator::Block<u8>& block)
{
    int result;
    ifilename filename = script->getScriptName();
    result = luaL_loadbuffer(m_state,
                             (const char *) block.data(),
                             be_checked_numcast<size_t >(block.count()-1),
                             filename.str().name);
    if (result == 0)
    {
        result = lua_pcall(m_state, 0, LUA_MULTRET, 0);
    }
    be_assert(result == 0, lua_tostring(m_state, -1));
}

}}
