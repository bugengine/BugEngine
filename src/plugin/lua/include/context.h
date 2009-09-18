/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_LUA_CONTEXT_H_
#define BE_LUA_CONTEXT_H_
/*****************************************************************************/

namespace BugEngine { namespace Lua
{

class Context
{
private:
    lua_State*      m_state;
    refptr<Logger>  m_logger;
public:
    Context();
    ~Context();

    void doFile(const char *filename);

private:
    static void* luaAlloc(void* ud, void* ptr, size_t osize, size_t nsize);


    static const luaL_Reg s_objectMetaTable[];
    static void printStack(lua_State* state);
    static Value get(lua_State* state, int index);
    static void push(lua_State* state, refptr<Object> o);
    static void push(lua_State* state, Object* o);
    static void push(lua_State* state, const Value& v);

    static int objectGC(lua_State* state);
    static int objectToString(lua_State *state);
    static int objectGet(lua_State *state);
    static int objectCall(lua_State *state);
};

}}

/*****************************************************************************/
#endif
