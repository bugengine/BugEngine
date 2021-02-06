/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_LUA_CONTEXT_H_
#define BE_LUA_CONTEXT_H_
/**************************************************************************************************/
#include <stdafx.h>
#include <bugengine/filesystem/file.script.hh>
#include <bugengine/plugin/plugin.hh>
#include <bugengine/rtti/classinfo.script.hh>
#include <bugengine/rtti/value.hh>
#include <bugengine/scriptengine.hh>
#include <lua.script.hh>

namespace BugEngine { namespace Lua {

class Context : public ScriptEngine< LuaScript >
{
private:
    lua_State* m_state;

public:
    Context(const Plugin::Context& context);
    ~Context();

    static minitl::format< 1024u > tostring(lua_State* state, int element);
    static void                    printStack(lua_State* l);
    static void typeError(lua_State* state, int narg, const char* expected, const char* got);
    static int  push(lua_State* state, const RTTI::Value& v);
    static void checkArg(lua_State* state, int narg, int type);
    static void checkArg(lua_State* state, int narg, const char* userDataType);
    static void checkArg(lua_State* state, int narg, const RTTI::Type& type);
    static minitl::format< 1024u > getCallInfo(lua_State* state);

private:
    virtual void unload(Resource::Resource& handle) override;
    virtual void runBuffer(weak< const LuaScript > script, Resource::Resource& resource,
                           const minitl::Allocator::Block< u8 >& buffer) override;
    virtual void reloadBuffer(weak< const LuaScript > script, Resource::Resource& resource,
                              const minitl::Allocator::Block< u8 >& buffer) override;

    static void* luaAlloc(void* ud, void* ptr, size_t osize, size_t nsize);
};

}}  // namespace BugEngine::Lua

/**************************************************************************************************/
#endif
