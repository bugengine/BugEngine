/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_WIN32_FILE_HH_
#define BE_SYSTEM_WIN32_FILE_HH_
/*****************************************************************************/
#include    <system/file/file.script.hh>

namespace BugEngine
{

class Win32File : public File
{
private:
    ifilename m_file;
public:
    Win32File(ifilename file, File::Media media, u64 size);
    ~Win32File();
private:
    void doFillBuffer(weak<File::Ticket> ticket) const override;
    virtual void doWriteBuffer(weak<Ticket> ticket) const override;
};

}

/*****************************************************************************/
#endif
