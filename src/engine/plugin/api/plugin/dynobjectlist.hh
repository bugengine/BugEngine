/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_PLUGIN_DYNOBJECTLIST_HH_
#define BE_PLUGIN_DYNOBJECTLIST_HH_
/*****************************************************************************/

namespace BugEngine { namespace Plugin
{

class DynamicObject;

#ifdef BE_STATIC

class be_api(PLUGIN) DynamicObjectList
{
    BE_NOCOPY(DynamicObjectList);
private:
    static const size_t         s_maxDynamicObjects = 64;
    static size_t               s_currentDynamicObject;
    static DynamicObjectList*   s_dynamicObjects[s_maxDynamicObjects];
private:
    struct SymbolPointer
    {
        void* data1;
        void* data2;
    };
    template< typename SYMBOL >
    union SymbolObject
    {
        SymbolPointer pointer;
        SYMBOL symbol;
    };
    struct Symbol
    {
        const char*     name;
        SymbolPointer   symbol;
        Symbol();
    };
private:
    const char* const   m_name;
    Symbol              m_symbols[16];
private:
    bool registerSymbolInternal(const char *name, SymbolPointer value);
    const SymbolPointer* findSymbolInternal(const char *name) const;
public:
    DynamicObjectList(const char* name);
    ~DynamicObjectList();

    template< typename SYMBOL >
    bool registerSymbol(SYMBOL symbol, const char *name)
    {
        SymbolObject<SYMBOL> s;
        s.symbol = symbol;
        return registerSymbolInternal(name, s.pointer);
    }
    template< typename SYMBOL >
    SYMBOL findSymbol(const char *name) const
    {
        const SymbolPointer* ptr = findSymbolInternal(name);
        if (ptr)
        {
            SymbolObject<SYMBOL> s;
            s.pointer = *ptr;
            return s.symbol;
        }
        else
        {
            return 0;
        }
    }
    static DynamicObjectList* findDynamicObject(const char *name);
};

#endif

}}

/*****************************************************************************/
#endif
