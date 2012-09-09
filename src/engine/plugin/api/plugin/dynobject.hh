/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_PLUGIN_DYNOBJECT_HH_
#define BE_PLUGIN_DYNOBJECT_HH_
/*****************************************************************************/
#include    <core/string/istring.hh>

namespace BugEngine { namespace Plugin
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
    static void*  getSymbolInternal(Handle handle, const istring& symbolName);
public:
    explicit DynamicObject(const inamespace &objectName, const ipath& objectPath);
    DynamicObject(const DynamicObject& other);
    ~DynamicObject();
    DynamicObject& operator=(const DynamicObject& other);

    operator const void*() const        { return m_handle; }
    bool operator!() const              { return m_handle == 0; }

    template< typename T >
    T* getSymbol(const istring& name) const
    {
        return (T*)getSymbolInternal(m_handle, name);
    }
};

}}

/*****************************************************************************/
#endif
