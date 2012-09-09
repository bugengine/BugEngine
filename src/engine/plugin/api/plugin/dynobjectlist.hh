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
    };
public:
    const char* const   name;
    Symbol              symbols[16];
public:
    DynamicObjectList(const char* name);
    ~DynamicObjectList();
    void registerSymbol(const char *name, void* value);
    static DynamicObjectList* findPlugin(const char *name);
};

#endif

}}

/*****************************************************************************/
#endif
