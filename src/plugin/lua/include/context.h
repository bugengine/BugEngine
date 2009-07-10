/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

#ifndef BE_LUA_CONTEXT_H_
#define BE_LUA_CONTEXT_H_
/*****************************************************************************/

namespace BugEngine { namespace Lua
{

class Context
{
private:
    lua_State*  m_state;
    Logger*     m_logger;
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
