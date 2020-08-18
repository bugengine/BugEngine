/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PLUGIN_DYNOBJECTLIST_HH_
#define BE_PLUGIN_DYNOBJECTLIST_HH_
/**************************************************************************************************/
#include <bugengine/plugin/stdafx.h>

namespace BugEngine { namespace Plugin {

class DynamicObject;

#ifdef BE_STATIC

class be_api(PLUGIN) DynamicObjectList
{
private:
    static DynamicObjectList* s_dynamicObjectRoot;

private:
    struct SymbolPointer
    {
        void* data1;
        void* data2;
    };
    template < typename SYMBOL >
    union SymbolObject
    {
        SymbolPointer pointer;
        SYMBOL        symbol;
    };
    struct Symbol
    {
        const char*   name;
        SymbolPointer symbol;
        Symbol();
    };

private:
    DynamicObjectList* m_next;
    const char* const  m_name;
    Symbol             m_symbols[16];

private:
    bool                 registerSymbolInternal(const char* name, SymbolPointer value);
    const SymbolPointer* findSymbolInternal(const char* name) const;

public:
    DynamicObjectList(const char* name);
    ~DynamicObjectList();

    template < typename SYMBOL >
    bool registerSymbol(SYMBOL symbol, const char* name)
    {
        SymbolObject< SYMBOL > s;
        s.symbol = symbol;
        return registerSymbolInternal(name, s.pointer);
    }
    template < typename SYMBOL >
    SYMBOL findSymbol(const char* name) const
    {
        const SymbolPointer* ptr = findSymbolInternal(name);
        if(ptr)
        {
            SymbolObject< SYMBOL > s;
            s.pointer = *ptr;
            return s.symbol;
        }
        else
        {
            return 0;
        }
    }
    static DynamicObjectList* findDynamicObject(const char* name);
    static void               showList();
};

#    define _BE_PLUGIN_EXPORT_VAR(var, value) static var = value;
#    define _BE_PLUGIN_EXPORT                 static
#    define _BE_REGISTER_PLUGIN_(id, name)                                                         \
        BE_EXPORT BugEngine::Plugin::DynamicObjectList s_plugin_##id(#name);
#    define _BE_REGISTER_PLUGIN(id, name) _BE_REGISTER_PLUGIN_(id, name)
#    define _BE_REGISTER_METHOD_(id, x, name)                                                      \
        BE_EXPORT bool s_symbol_##id##_##x = s_plugin_##id.registerSymbol(&x, #name);
#    define _BE_REGISTER_METHOD(id, x)             _BE_REGISTER_METHOD_(id, x, x)
#    define _BE_REGISTER_METHOD_NAMED(id, x, name) _BE_REGISTER_METHOD_(id, x, name)

#else

#    define _BE_PLUGIN_EXPORT_VAR(var, value)                                                      \
        _BE_PLUGIN_EXPORT var;                                                                     \
        var = value;
#    define _BE_PLUGIN_EXPORT extern "C" BE_EXPORT
#    define _BE_REGISTER_PLUGIN(id, name)
#    define _BE_REGISTER_METHOD(id, x)
#    define _BE_REGISTER_METHOD_NAMED(id, x, name)

#endif

}}  // namespace BugEngine::Plugin

/**************************************************************************************************/
#endif
