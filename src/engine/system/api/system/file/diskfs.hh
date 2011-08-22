/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_FILE_DISKFS_HH_
#define BE_SYSTEM_FILE_DISKFS_HH_
/*****************************************************************************/
#include    <system/file/fscomponent.hh>
#include    <core/string/istring.hh>


namespace BugEngine
{

class IMemoryStream;


class be_api(SYSTEM) DiskFS : public FileSystemComponent
{
    friend class FileSystem;
private:
    ipath       m_prefix;
    bool        m_readOnly;
public:
    enum OpenMode { Normal, CreateRoot, ReadOnly };
    DiskFS(const ipath& prefix, OpenMode mode = Normal);
    ~DiskFS(void);

    virtual bool writable() const override;

    virtual ref<IMemoryStream> open(const ifilename& file, FileOpenMode mode) const override;
    virtual size_t age(const ifilename& file) const override;
};

}


/*****************************************************************************/
#endif
