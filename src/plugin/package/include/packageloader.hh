/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_PACKAGE_PACKAGELOADER_HH_
#define BE_PACKAGE_PACKAGELOADER_HH_
/*****************************************************************************/
#include    <system/file/folder.script.hh>
#include    <package.script.hh>

namespace BugEngine { namespace PackageManager
{

class PackageLoader : public minitl::refcountable
{
    BE_NOCOPY(PackageLoader);
private:
    weak<const Folder> const    m_dataFolder;
public:
    PackageLoader();
    ~PackageLoader();

    void loadPackage(weak<const Package> package);
    void unloadPackage(const ResourceHandle& handle);
public:
    void* operator new(size_t size, void* where)     { return ::operator new(size, where); }
    void  operator delete(void* memory, void* where) { ::operator delete(memory, where); }
    void  operator delete(void* memory)              { be_notreached(); ::operator delete(memory); }
};

}}

/*****************************************************************************/
#endif
