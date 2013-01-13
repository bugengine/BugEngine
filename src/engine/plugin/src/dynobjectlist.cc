/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <plugin/stdafx.h>
#include    <plugin/dynobject.hh>
#include    <plugin/dynobjectlist.hh>
#include    <cstring>

namespace BugEngine { namespace Plugin
{

#ifdef BE_STATIC

size_t              DynamicObjectList::s_currentDynamicObject;

DynamicObjectList*  DynamicObjectList::s_dynamicObjects[DynamicObjectList::s_maxDynamicObjects];

DynamicObjectList::Symbol::Symbol()
    :   name(0)
    ,   symbol()
{
}

DynamicObjectList::DynamicObjectList(const char* name)
    :   m_name(name)
{
    be_assert(s_currentDynamicObject < s_maxDynamicObjects, "too many dynamic objects registered; increase the value of DynamicObjectList::s_maxDynamicObjects to more than %d" | s_maxDynamicObjects);
    be_info("Registering built-in dynamic object %s" | name);
    s_dynamicObjects[s_currentDynamicObject++] = this;
}

DynamicObjectList::~DynamicObjectList()
{
    for (size_t i = 0; i < s_currentDynamicObject; ++i)
    {
        if (s_dynamicObjects[i] == this)
        {
            s_dynamicObjects[i] = s_dynamicObjects[--s_currentDynamicObject];
            return;
        }
    }
    be_notreached();
}

DynamicObjectList* DynamicObjectList::findDynamicObject(const char *name)
{
    be_info("loading dynamic object %s (built-in)" | name);
    for (size_t i = 0; i < s_currentDynamicObject; ++i)
    {
        if (strcmp(name, s_dynamicObjects[i]->m_name) == 0)
        {
            return s_dynamicObjects[i];
        }
    }
    be_info("unable to load dynamic object %s" | name);
    return 0;
}

bool DynamicObjectList::registerSymbolInternal(const char* name, SymbolPointer value)
{
    for (u32 i = 0; i < sizeof(m_symbols)/sizeof(m_symbols[0]); ++i)
    {
        if (m_symbols[i].name == 0)
        {
            m_symbols[i].name = name;
            m_symbols[i].symbol = value;
            return true;
        }
    }
    be_notreached();
    return false;
}

const DynamicObjectList::SymbolPointer* DynamicObjectList::findSymbolInternal(const char* name) const
{
    for (u32 i = 0; i < sizeof(m_symbols)/sizeof(m_symbols[0]); ++i)
    {
        if (strcmp(m_symbols[i].name, name) == 0)
        {
            return &m_symbols[i].symbol;
        }
    }
    return 0;
}


DynamicObject::Handle DynamicObject::load(const inamespace& objectName, const ipath& objectPath)
{
    return (Handle)DynamicObjectList::findDynamicObject(objectName.str().name);
}

void DynamicObject::unload(Handle handle)
{
}

void* DynamicObject::getSymbolInternal(Handle handle, const istring& name)
{
    return reinterpret_cast<DynamicObjectList*>(handle)->findSymbol<void*>(name.c_str());
}

#endif

}}
