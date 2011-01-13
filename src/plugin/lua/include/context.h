/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_LUA_CONTEXT_H_
#define BE_LUA_CONTEXT_H_
/*****************************************************************************/
#include    <rtti/scripting.hh>
#include    <rtti/value.hh>
#include    <system/filesystem.hh>

namespace BugEngine { namespace Lua
{

class Context : public Scripting
{
private:
    lua_State*              m_state;
    weak<const FileSystem>  m_filesystem;
    ref<Logger>             m_logger;
public:
    Context(weak<const FileSystem> filesystem, Value root);
    ~Context();

    void doFile(const ifilename& file) override;
    void doFile(weak<IMemoryStream> stream, const char *filename) override;

private:
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
