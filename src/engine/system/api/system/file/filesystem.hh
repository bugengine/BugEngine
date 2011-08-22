/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_FILESYSTEM_HH_
#define BE_SYSTEM_FILESYSTEM_HH_
/*****************************************************************************/
#include    <core/string/istring.hh>

namespace BugEngine
{

class FileSystemComponent;
class IMemoryStream;


enum FileOpenMode
{
    eReadOnly,
    eReadWrite
};

class be_api(SYSTEM) FileSystem : public minitl::refcountable
{
    BE_NOCOPY(FileSystem);
private:
    class FileSystemMountPoint : public pointer
    {
    public:
        typedef minitl::hashmap< istring, scoped<FileSystemMountPoint> > ChildrenMap;
    private:
        ref<const FileSystemComponent>  m_component;
        ChildrenMap                     m_children;
    public:
        FileSystemMountPoint();
        FileSystemMountPoint(ref<const FileSystemComponent> component);
        ~FileSystemMountPoint();

        weak<FileSystemMountPoint> getOrCreate(const istring& child);
        weak<FileSystemMountPoint> get(const istring& child);
        void erase(const istring& child);

        void mount(const ref<const FileSystemComponent>& component);
        void umount();
        weak<const FileSystemComponent>  component() const;

        bool empty() const;
    };
private:
    scoped<FileSystemMountPoint> m_root;
public:
    FileSystem(void);
    ~FileSystem(void);

    void mount(const ipath& prefix, ref<const FileSystemComponent> component);
    void umount(const ipath& prefix);
    ref<IMemoryStream> open(const ifilename& file, FileOpenMode mode) const;
    size_t age(const ifilename& file) const;

    minitl::vector<ifilename> listFiles(const ipath& prefix, const char* extension = 0);
    minitl::vector<ipath> listDirectories(const ipath& prefix);
};

}
#include    <system/file/fscomponent.hh>




/*****************************************************************************/
#endif
