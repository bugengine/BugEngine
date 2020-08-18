/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin/stdafx.h>
#include <bugengine/plugin/dynobject.hh>
#include <bugengine/plugin/dynobjectlist.hh>
#include <cstring>

namespace BugEngine { namespace Plugin {

#ifdef BE_STATIC

DynamicObjectList* DynamicObjectList::s_dynamicObjectRoot = 0;

DynamicObjectList::Symbol::Symbol() : name(0), symbol()
{
}

DynamicObjectList::DynamicObjectList(const char* name) : m_next(s_dynamicObjectRoot), m_name(name)
{
    be_info("Registering built-in dynamic object %s" | name);
    s_dynamicObjectRoot = this;
}

DynamicObjectList::~DynamicObjectList()
{
    be_assert(s_dynamicObjectRoot == this, "Invalid order in create/destroy dynamic object");
    s_dynamicObjectRoot = m_next;
}

DynamicObjectList* DynamicObjectList::findDynamicObject(const char* name)
{
    be_info("loading dynamic object %s (built-in)" | name);
    DynamicObjectList* current = s_dynamicObjectRoot;
    while(current)
    {
        if(strcmp(name, current->m_name) == 0)
        {
            return current;
        }
        current = current->m_next;
    }
    be_info("unable to load dynamic object %s" | name);
    return 0;
}

bool DynamicObjectList::registerSymbolInternal(const char* name, SymbolPointer value)
{
    for(u32 i = 0; i < sizeof(m_symbols) / sizeof(m_symbols[0]); ++i)
    {
        if(m_symbols[i].name == 0)
        {
            m_symbols[i].name   = name;
            m_symbols[i].symbol = value;
            return true;
        }
    }
    be_notreached();
    return false;
}

const DynamicObjectList::SymbolPointer*
DynamicObjectList::findSymbolInternal(const char* name) const
{
    for(u32 i = 0; i < sizeof(m_symbols) / sizeof(m_symbols[0]); ++i)
    {
        if(strcmp(m_symbols[i].name, name) == 0)
        {
            return &m_symbols[i].symbol;
        }
    }
    return 0;
}

void DynamicObjectList::showList()
{
    DynamicObjectList* object = s_dynamicObjectRoot;
    while(object)
    {
        be_info("registered built-in plugin %s" | object->m_name);
        object = object->m_next;
    }
}

DynamicObject::Handle DynamicObject::load(const inamespace& objectName, const ipath& objectPath)
{
    be_forceuse(objectPath);
    return (Handle)DynamicObjectList::findDynamicObject(objectName.str().name);
}

void DynamicObject::unload(Handle handle)
{
    be_forceuse(handle);
}

void* DynamicObject::getSymbolInternal(Handle handle, const istring& name)
{
    return reinterpret_cast< DynamicObjectList* >(handle)->findSymbol< void* >(name.c_str());
}

#endif

}}  // namespace BugEngine::Plugin
