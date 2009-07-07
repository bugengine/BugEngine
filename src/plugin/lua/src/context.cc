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

#include    <stdafx.h>

#include    <context.h>

#include    <system/filesystem.hh>
#include    <core/memory/streams.hh>
#include    <rtti/namespace.hh>


namespace BugEngine { namespace Lua
{

const luaL_Reg Context::s_objectMetaTable[] = {
    {"__gc",       Context::objectGC},
    {"__tostring", Context::objectToString},
    {"__index", Context::objectGet},
    {0, 0}
};

void* Context::luaAlloc(void* ud, void* ptr, size_t osize, size_t nsize)
{
    UNUSED(ud);
    UNUSED(osize);
    if(nsize)
    {
        if(osize)
        {
            return Malloc::internalRealloc(ptr, nsize, 16);
        }
        else
        {
            return Malloc::internalAlloc(nsize, 16);
        }
    }
    else
    {
        Malloc::internalFree(ptr);
        return 0;
    }
}

Context::Context()
:   m_state(lua_newstate(&Context::luaAlloc, 0))
{
    luaopen_base(m_state);
    luaopen_table(m_state);
    luaopen_string(m_state);
    luaopen_math(m_state);
    luaopen_debug(m_state);

    luaL_register(m_state, "bugrefobject", s_objectMetaTable);
    luaL_register(m_state, "bugweakobject", &s_objectMetaTable[1]);

    push(m_state, Namespace::root());
    lua_setglobal(m_state, "BugEngine");
    lua_pop(m_state, 1);
}

Context::~Context()
{
    lua_close(m_state);
}

void Context::doFile(const char *filename)
{
    BugEngine::FileSystem* fs = BugEngine::FileSystem::instance();
    refptr<BugEngine::AbstractMemoryStream> file = fs->open(filename, BugEngine::eReadOnly);
    if(file)
    {
        int result;
        result = luaL_loadbuffer(m_state, (const char *)file->basememory(), (size_t)file->size(), filename);
        if(result == 0)
        {
            result = lua_pcall(m_state, 0, LUA_MULTRET, 0);
        }
        AssertMsg(result == 0, lua_tostring(m_state, -1));
    }
}

void Context::push(lua_State* state, refptr<Object> o)
{
    Object** userdata = (Object**)lua_newuserdata(state, sizeof(Object*));
    *userdata = o.get();
    minitl::addref(*userdata);
    lua_getglobal(state, "bugrefobject");
    lua_setmetatable(state, -2);
}

void Context::push(lua_State* state, Object* o)
{
    Object** userdata = (Object**)lua_newuserdata(state, sizeof(Object*));
    *userdata = o;
    lua_getglobal(state, "bugweakobject");
    lua_setmetatable(state, -2);
}

void Context::push(lua_State* state, const Value& v)
{

    switch(v.type())
    {
    case RTTI::PropertyTypeBool:
        lua_pushboolean(state, v.as<bool>());
        break;
    case RTTI::PropertyTypeInteger:
        lua_pushinteger(state, (lua_Integer)v.as<i64>());
        break;
    case RTTI::PropertyTypeUnsigned:
        lua_pushinteger(state, (lua_Integer)v.as<u64>());
        break;
    case RTTI::PropertyTypeFloat:
        lua_pushnumber(state, v.as<double>());
        break;
    case RTTI::PropertyTypeString:
        lua_pushstring(state, v.as<std::string>().c_str());
        break;
    case RTTI::PropertyTypeObject:
        push(state, v.as< refptr<Object> >());
        break;
    case RTTI::PropertyTypeWeakObject:
        push(state, v.as< Object* >());
        break;
    }

}

int Context::objectGC(lua_State *state)
{
    Object** userdata = (Object**)lua_touserdata(state, -1);
    minitl::decref(*userdata);
    return 0;
}

int Context::objectToString(lua_State *state)
{
    Object** userdata = (Object**)lua_touserdata(state, -1);
    lua_pushfstring(state, "[%s object @0x%p]", (*userdata)->metaclass()->name().c_str(), (*userdata));
    return 1;
}

int Context::objectGet(lua_State *state)
{
    Object** userdata = (Object**)lua_touserdata(state, -2);
    if(!*userdata)
    {
        lua_pushnil(state);
        return 1;
    }
    const char *name = lua_tostring(state, -1);
    const Property* p = (*userdata)->metaclass()->getProperty(name);
    if(!p)
    {
        lua_pushnil(state);
        return 1;
    }
    Value v = p->get(*userdata);
    push(state, v);
    return 1;
}

}}
