/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_FILESYSTEM_HH_
#define BE_SYSTEM_FILESYSTEM_HH_
/*****************************************************************************/
#include    <core/string/istring.hh>
#include    <core/debug/exception.hh>

namespace BugEngine
{

class FileSystemComponent;
class AbstractMemoryStream;


enum FileOpenMode
{
    eReadOnly,
    eReadWrite
};

//lint -esym(1712,EFileNotFound)
class EFileNotFound : public EException
{
public:
    EFileNotFound(const ifilename& file) :
        EException("file " + file.str() + " does not exist")
    {
    }
};

//lint -esym(1712,ENoCreate)
class ENoCreate : public EException
{
public:
    ENoCreate(const ifilename& file) :
        EException("cannot create file " + file.str())
    {
    }
};

//lint -esym(1712,EReadOnly)
class EReadOnly : public EException
{
public:
    EReadOnly(const ifilename& file) :
        EException("cannot open file " + file.str() + " in read-write mode")
    {
    }
};

class be_api(SYSTEM) FileSystem : public minitl::refcountable
{
private:
    class FileSystemMountPoint : public pointer
    {
    public:
        typedef std::map< istring, scoped<FileSystemMountPoint> > ChildrenMap;
    private:
        ref<const FileSystemComponent>  m_component;
        ChildrenMap                     m_children;
    public:
        FileSystemMountPoint(ref<const FileSystemComponent> component = ref<const FileSystemComponent>());
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
    static weak<FileSystem> instance();
    FileSystem(void);
    ~FileSystem(void);

    void mount(const ipath& prefix, ref<const FileSystemComponent> component);
    void umount(const ipath& prefix);
    ref<AbstractMemoryStream> open(const ifilename& file, FileOpenMode mode) const;
    size_t age(const ifilename& file) const;

    std::set<ifilename> listFiles(const ipath& prefix, const char* extension = 0);
    std::set<ipath> listDirectories(const ipath& prefix);
};

}
#include    <system/fscomponent.hh>




/*****************************************************************************/
#endif
