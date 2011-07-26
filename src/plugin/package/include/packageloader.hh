/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_PACKAGE_PACKAGELOADER_HH_
#define BE_PACKAGE_PACKAGELOADER_HH_
/*****************************************************************************/
#include    <system/filesystem.hh>
#include    <system/resource/resource.script.hh>

namespace BugEngine { namespace Plugin { namespace PackageManager
{

class PackageLoader : public minitl::refcountable
{
    BE_NOCOPY(PackageLoader);
private:
    weak<const FileSystem> const    m_filesystem;
public:
    PackageLoader(weak<const FileSystem> filesystem);
    ~PackageLoader();
public:
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
    void  operator delete(void* memory, void* where) { return ::operator delete(memory, where); }
    void  operator delete(void* memory)              { be_notreached(); return ::operator delete(memory); }
};

}}}

/*****************************************************************************/
#endif
