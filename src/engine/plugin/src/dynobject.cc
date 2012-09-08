/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <plugin/stdafx.h>
#include    <plugin/dynobject.hh>

#include "Platform.h"

namespace BugEngine { namespace Plugin
{

DynamicObject::DynamicObject(const inamespace& objectName, const ipath& objectPath)
    :   m_handle(load(objectName, objectPath))
    ,   m_refCount(new (Arena::general()) i_u32(i_u32::One))
{
}

DynamicObject::DynamicObject(const DynamicObject& other)
    :   m_handle(other.m_handle)
    ,   m_refCount(other.m_refCount)
{
    ++*m_refCount;
}

DynamicObject::~DynamicObject()
{
    if (--*m_refCount == 0)
    {
        unload(m_handle);
        Arena::general().free(m_refCount);
    }
}

#ifdef BE_STATIC

size_t              DynamicObjectList::s_currentDynamicObject;

DynamicObjectList*  DynamicObjectList::s_dynamicObjects[s_maxDynamicObject];

DynamicObjectList::DynamicObjectList(const char* name)
    :   name(name)
{
    be_assert(s_currentDynamicObject < s_maxDynamicObject, "too many dynamic objects registered; increase the value of DynamicObjectList::s_maxDynamicObject to more than %d" | s_maxDynamicObject);
    be_info("Registering built-in dynamic object %s" | name);
    s_dynamicObjects[s_maxDynamicObject++] = this;
    memset(m_symbols, 0, sizeof(m_symbols));
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
        if (strcmp(name, s_dynamicObjects[i]->name) == 0)
        {
            return s_dynamicObjects[i];
        }
    }
    return 0;
}

void DynamicObjectList::registerSymbol(const char* name, void* value)
{
    for (u32 i = 0; i < sizeof(m_symbols)/sizeof(m_symbols[0]); ++i)
    {
        if (m_symbols[i] == 0)
        {
            m_symbols[i].name = name;
            m_symbols[i].value = value;
            return;
        }
    }
    be_notreached();
}

void* DynamicObjectList::findSymbol(const char* name)
{
    for (u32 i = 0; i < sizeof(m_symbols)/sizeof(m_symbols[0]); ++i)
    {
        if ((m_symbols[i], name) == 0)
        {
            return m_symbols[i].value = value;
        }
    }
    return 0;
}


DynamicObject::Handle DynamicObject::load(const inamespace& objectName, const inamespace& objectPath)
{
    return (Handle)DynamicObjectList::findDynamicObject(objectName.str().name);
}

void DynamicObject::unload(Handle handle)
{
}

void* DynamicObject::getSymbolInternal(Handle handle, const char* name)
{
    return (DynamicObjectList*)handle->findSymbol(name);
}

#endif

}}


