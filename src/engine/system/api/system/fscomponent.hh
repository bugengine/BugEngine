/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_FILESYSTEM_COMPONENT_HH_
#define BE_SYSTEM_FILESYSTEM_COMPONENT_HH_
/*****************************************************************************/
#include    <minitl/ptr/refcountable.hh>
#include    <system/filesystem.hh>


namespace BugEngine
{

class AbstractMemoryStream;

    
class SYSTEMEXPORT FileSystemComponent : public minitl::refcountable<void>
{
    friend class FileSystem;
public:
    ~FileSystemComponent();
protected:
    FileSystemComponent();

    virtual bool writable() const = 0;

    virtual refptr<AbstractMemoryStream> open(const ifilename& file, FileOpenMode mode) const = 0;
    virtual size_t age(const ifilename& file) const = 0;
};

}


/*****************************************************************************/
#endif
