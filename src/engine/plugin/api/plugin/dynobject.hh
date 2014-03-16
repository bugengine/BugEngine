/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PLUGIN_DYNOBJECT_HH_
#define BE_PLUGIN_DYNOBJECT_HH_
/**************************************************************************************************/
#include    <plugin/stdafx.h>
#include    <core/string/istring.hh>

namespace BugEngine { namespace Plugin
{

class be_api(PLUGIN) DynamicObject
{
private:
    template< typename T >
    union Symbol
    {
        void* pointer;
        T* symbol;
    };
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
        Symbol<T> s;
        s.pointer = getSymbolInternal(m_handle, name);
        return s.symbol;
    }
};

}}

/**************************************************************************************************/
#endif
