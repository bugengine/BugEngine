/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_FILESYSTEM_WIN32_FILE_HH_
#define BE_FILESYSTEM_WIN32_FILE_HH_
/*****************************************************************************/
#include    <filesystem/file.script.hh>

namespace BugEngine
{

class Win32File : public File
{
public:
    Win32File(ifilename file, File::Media media, u64 size, u64 timestamp);
    ~Win32File();

    void refresh(u64 size, u64 timestamp);
private:
    void doFillBuffer(weak<File::Ticket> ticket) const override;
    virtual void doWriteBuffer(weak<Ticket> ticket) const override;
};

}

/*****************************************************************************/
#endif
