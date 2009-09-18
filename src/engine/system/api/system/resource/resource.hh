/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_RESOURCE_RESOURCE_HH_
#define BE_SYSTEM_RESOURCE_RESOURCE_HH_
/*****************************************************************************/

namespace BugEngine
{

class MemoryStream;
class ifilename;

template< typename Type, typename Pipeline = typename Type::Pipeline >
class Resource
{
private:
    Type*       m_resource;
    Pipeline*   m_loader;
protected:
    void load(Pipeline* loader, const ifilename& file);
    void unload();
public:
    Resource();
    Resource(Pipeline* loader, const ifilename& file);
    ~Resource();

    inline bool isBound() const;

    inline Type* get();

    inline operator void*() const;
    inline Type* operator->();
    inline const Type* operator->() const;
    inline Type& operator*();
    inline const Type& operator*() const;
};

}

#include "resource.inl"

/*****************************************************************************/
#endif
