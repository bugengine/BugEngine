/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_FILESYSTEM_POSIX_FILE_HH_
#define BE_FILESYSTEM_POSIX_FILE_HH_
/*****************************************************************************/
#include    <filesystem/file.script.hh>
#include    <sys/types.h>

namespace BugEngine
{

class PosixFile : public File
{
public:
    PosixFile(ifilename file, File::Media media, u64 size, time_t modifiedTime);
    ~PosixFile();

    void refresh(u64 size, time_t modifiedTime);
private:
    virtual void doFillBuffer(weak<File::Ticket> ticket) const override;
    virtual void doWriteBuffer(weak<Ticket> ticket) const;
};

}

/*****************************************************************************/
#endif
