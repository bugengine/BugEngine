/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_LUA_CONTEXT_H_
#define BE_LUA_CONTEXT_H_
/*****************************************************************************/
#include    <bugengine/scriptengine.hh>
#include    <rtti/classinfo.script.hh>
#include    <rtti/value.inl>
#include    <filesystem/file.script.hh>
#include    <plugin/plugin.hh>
#include    <luascript.script.hh>

namespace BugEngine { namespace Lua
{

class Context : public ScriptEngine<LuaScript>
{
private:
    lua_State*  m_state;
    ref<Logger> m_logger;
public:
    Context(const PluginContext& context);
    ~Context();
private:
    void runBuffer(weak<const LuaScript> resource, const minitl::Allocator::Block<u8>& buffer) override;

    static void* luaAlloc(void* ud, void* ptr, size_t osize, size_t nsize);


    static const luaL_Reg s_valueMetaTable[];
};

}}

/*****************************************************************************/
#endif
