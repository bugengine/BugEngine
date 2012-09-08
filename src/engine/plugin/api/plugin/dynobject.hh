/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_PLUGIN_DYNOBJECT_HH_
#define BE_PLUGIN_DYNOBJECT_HH_
/*****************************************************************************/

namespace BugEngine
{

class DynamicObject
{
private:
    typedef void*   Handle;
private:
    Handle          m_handle;
    i_u32*          m_refCount;
private:
    static Handle load(const inamespace& objectName, const ipath& objectPath);
    static void   unload(Handle handle);
    static void*  getSymbolInternal(Handle handle, const istring& symbolName) const;
public:
    explicit DynamicObject(const inamespace &objectName, const ipath& objectPath);
    DynamicObject(const DynamicObject& other);
    ~DynamicObject();
    DynamicObject& operator=(const Plugin& other);

    operator const void*() const        { return m_handle; }
    bool operator!() const              { return m_handle == 0; }

    template< typename T >
    T* getSymbol(const istring& name) const
    {
        return (T*)getSymbolInternal(m_handle, name);
    }
};

#ifdef BE_STATIC

class be_api(PLUGIN) DynamicObjectList
{
    BE_NOCOPY(DynamicObjectList);
private:
    static const size_t s_maxDynamicObjects = 64;
    static size_t       s_currentDynamicObject;
    static PluginList*  s_dynamicObjects[s_maxDynamicObjects];
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

}

/*****************************************************************************/
#endif
