/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_POSIX_FILE_HH_
#define BE_SYSTEM_POSIX_FILE_HH_
/*****************************************************************************/
#include <system/file/file.script.hh>

namespace BugEngine
{

class PosixFile : public File
{
private:
    ifilename m_file;
public:
    PosixFile(ifilename file, File::Media media, u64 size);
    ~PosixFile();
private:
    virtual void doFillBuffer(weak<File::Ticket> ticket) const override;
    virtual void doWriteBuffer(weak<Ticket> ticket) const;
};

}

/*****************************************************************************/
#endif
