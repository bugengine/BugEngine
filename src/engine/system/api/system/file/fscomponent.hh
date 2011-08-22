/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_FILE_FSCOMPONENT_HH_
#define BE_SYSTEM_FILE_FSCOMPONENT_HH_
/*****************************************************************************/
#include    <minitl/ptr/refcountable.hh>
#include    <system/file/filesystem.hh>


namespace BugEngine
{

class IMemoryStream;

class be_api(SYSTEM) FileSystemComponent : public minitl::refcountable
{
    friend class FileSystem;
    BE_NOCOPY(FileSystemComponent);
public:
    ~FileSystemComponent();
protected:
    FileSystemComponent();

    virtual bool writable() const = 0;

    virtual ref<IMemoryStream> open(const ifilename& file, FileOpenMode mode) const = 0;
    virtual size_t age(const ifilename& file) const = 0;
};

}


/*****************************************************************************/
#endif
