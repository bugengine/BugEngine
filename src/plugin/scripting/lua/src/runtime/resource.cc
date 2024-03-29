/* BugEngine <bugengine.devel@gmail.com>
 see LICENSE for detail */

#include <stdafx.h>

#include <bugengine/resource/resourcemanager.hh>
#include <context.hh>
#include <runtime/resource.hh>

namespace BugEngine { namespace Lua {

struct ResourceToken
{
    weak< Resource::ResourceManager >   manager;
    raw< const Meta::Class >            type;
    weak< const Resource::Description > description;
};

extern "C" int resourceLoaderGC(lua_State* state)
{
    Context::checkArg(state, 1, "BugEngine.ResourceManager");
    weak< Resource::ResourceManager >* userdata
        = (weak< Resource::ResourceManager >*)lua_touserdata(state, 1);
    userdata->~weak();
    return 0;
}

extern "C" int resourceLoaderToString(lua_State* state)
{
    Context::checkArg(state, 1, "BugEngine.ResourceManager");
    weak< Resource::ResourceManager >* userdata
        = (weak< Resource::ResourceManager >*)lua_touserdata(state, 1);
    lua_pushfstring(state, "resourcemanager[%p]", userdata->operator->());
    return 1;
}

extern "C" int resourceLoaderLoad(lua_State* state)
{
    Context::checkArg(state, 1, "BugEngine.ResourceManager");
    Context::checkArg(state, 2, "BugEngine.Object");

    weak< Resource::ResourceManager > userdata
        = *(weak< Resource::ResourceManager >*)lua_touserdata(state, 1);
    Meta::Value*                        v = (Meta::Value*)lua_touserdata(state, 2);
    weak< const Resource::Description > description
        = v->as< weak< const Resource::Description > >();
    ResourceToken* resourceToken = (ResourceToken*)lua_newuserdata(state, sizeof(ResourceToken));
    new((void*)resourceToken) ResourceToken;
    resourceToken->description = description;
    resourceToken->manager     = userdata;
    resourceToken->type        = v->type().metaclass;
    userdata->load(resourceToken->type, description);
    luaL_getmetatable(state, "BugEngine.Resource");
    lua_setmetatable(state, -2);
    return 1;
}

const luaL_Reg s_resourceLoaderMetaTable[] = {{"__gc", resourceLoaderGC},
                                              {"__tostring", resourceLoaderToString},
                                              {"load", resourceLoaderLoad},
                                              {0, 0}};

extern "C" int resourceGC(lua_State* state)
{
    Context::checkArg(state, 1, "BugEngine.Resource");
    ResourceToken* userdata = (ResourceToken*)lua_touserdata(state, 1);
    userdata->manager->unload(userdata->type, userdata->description);
    userdata->~ResourceToken();
    return 0;
}

extern "C" int resourceToString(lua_State* state)
{
    Context::checkArg(state, 1, "BugEngine.Resource");
    ResourceToken* userdata = (ResourceToken*)lua_touserdata(state, 1);
    lua_pushfstring(state, "Resource<%s>[%p]", userdata->type->fullname().str().name,
                    userdata->description.operator->());
    return 1;
}

const luaL_Reg s_resourceMetaTable[]
    = {{"__gc", resourceGC}, {"__tostring", resourceToString}, {0, 0}};

}}  // namespace BugEngine::Lua
