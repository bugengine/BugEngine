/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_LUA_CONTEXT_H_
#define BE_LUA_CONTEXT_H_
/*****************************************************************************/
#include    <main/scripting.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/value.inl>
#include    <system/file/file.script.hh>

namespace BugEngine { namespace Lua
{

class Context : public IScriptEngine
{
private:
    lua_State*                                  m_state;
    ref<Logger>                                 m_logger;
public:
    Context();
    ~Context();
private:
    void addNamespace(istring name, raw<const RTTI::ClassInfo> ns) override;
    void runBuffer(const Allocator::Block<u8>& buffer) override;

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
