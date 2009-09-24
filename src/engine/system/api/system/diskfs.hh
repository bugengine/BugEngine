/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_FILESYSTEM_DISK_HH_
#define BE_SYSTEM_FILESYSTEM_DISK_HH_
/*****************************************************************************/
#include    <system/fscomponent.hh>
#include    <core/string/istring.hh>


namespace BugEngine
{

class AbstractMemoryStream;


class be_api(SYSTEM) DiskFS : public FileSystemComponent
{
    friend class FileSystem;
private:
    ipath       m_prefix;
    bool        m_readOnly;
public:
    DiskFS(const ipath& prefix, bool readonly = false);
    ~DiskFS(void);

    virtual bool writable() const override;

    virtual refptr<AbstractMemoryStream> open(const ifilename& file, FileOpenMode mode) const override;
    virtual size_t age(const ifilename& file) const override;
};

}


/*****************************************************************************/
#endif
