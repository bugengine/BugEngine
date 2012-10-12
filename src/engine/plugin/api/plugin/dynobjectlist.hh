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
    struct Symbol
    {
        const char* name;
        void*       symbol;
        Symbol();
    };
private:
    const char* const   m_name;
    Symbol              m_symbols[16];
private:
    bool registerSymbolInternal(const char *name, void* value);
public:
    DynamicObjectList(const char* name);
    ~DynamicObjectList();

    template< typename SYMBOL >
    bool registerSymbol(SYMBOL symbol, const char *name)
    {
        return registerSymbolInternal(name);
    }
    void* findSymbol(const char *name) const;
    static DynamicObjectList* findDynamicObject(const char *name);
};

#endif

}}

/*****************************************************************************/
#endif
