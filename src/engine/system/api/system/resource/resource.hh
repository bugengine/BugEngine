/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_RESOURCE_RESOURCE_HH_
#define BE_SYSTEM_RESOURCE_RESOURCE_HH_
/*****************************************************************************/

namespace BugEngine
{

template< typename Destination, typename Source, typename ResourceLoader = typename Source::ResourceLoader >
class Resource
{
private:
    ref<Destination>                    m_resource;
    weak<const ResourceLoader> const    m_loader;
protected:
    void load(const Source& source);
    void unload();
public:
    Resource(weak<const ResourceLoader> loader, const Source& source);
    ~Resource();

    inline bool isBound() const;

    inline weak<Destination> get();

    inline operator const void*() const;
    inline bool operator!() const;
    inline Destination* operator->();
    inline const Destination* operator->() const;
};

}

#include "resource.inl"

/*****************************************************************************/
#endif
