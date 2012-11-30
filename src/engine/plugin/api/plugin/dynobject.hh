/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_PLUGIN_DYNOBJECT_HH_
#define BE_PLUGIN_DYNOBJECT_HH_
/*****************************************************************************/
#include    <core/string/istring.hh>

namespace BugEngine { namespace Plugin
{

class be_api(PLUGIN) DynamicObject
{
private:
    typedef void*   Handle;
private:
    Handle          m_handle;
private:
    static Handle load(const inamespace& objectName, const ipath& objectPath);
    static void   unload(Handle handle);
    static void*  getSymbolInternal(Handle handle, const istring& symbolName);
public:
    DynamicObject(const inamespace &objectName, const ipath& objectPath);
    ~DynamicObject();

    operator const void*() const        { return m_handle; }
    bool operator!() const              { return m_handle == 0; }

    template< typename T >
    T* getSymbol(const istring& name) const
    {
        return (T*)(getSymbolInternal(m_handle, name));
    }
};

}}

/*****************************************************************************/
#endif
