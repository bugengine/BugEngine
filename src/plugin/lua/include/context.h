/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_LUA_CONTEXT_H_
#define BE_LUA_CONTEXT_H_
/*****************************************************************************/
#include    <main/scriptengine.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/value.inl>
#include    <system/file/file.script.hh>

namespace BugEngine { namespace Lua
{

be_tag(ResourceLoaders())
class LuaScript : public Script
{
    friend class PackageLoader;
published:
    LuaScript(be_tag(EditHint::Extension(".lua")) ref<const File> file);
    ~LuaScript();
};


class Context : public ScriptEngine<LuaScript>
{
private:
    lua_State*                                  m_state;
    ref<Logger>                                 m_logger;
public:
    Context();
    ~Context();
private:
    void runBuffer(weak<const LuaScript> resource, const Allocator::Block<u8>& buffer) override;

    static void* luaAlloc(void* ud, void* ptr, size_t osize, size_t nsize);


    static const luaL_Reg s_valueMetaTable[];
    static void printStack(lua_State* state);
    static Value get(lua_State* state, int index);
    static void push(lua_State* state, const Value& v);

    static int valueGC(lua_State* state);
    static int valueToString(lua_State *state);
    static int valueGet(lua_State *state);
    static int valueCall(lua_State *state);
};

}}

/*****************************************************************************/
#endif
