/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_FILESYSTEM_ZIPFILE_HH_
#define BE_FILESYSTEM_ZIPFILE_HH_
/**************************************************************************************************/
#include <bugengine/filesystem/stdafx.h>
#include <bugengine/filesystem/file.script.hh>

#include <cstdlib>

#include <cstdio>
#include <unzip.h>

namespace BugEngine {

class ZipFile : public File
{
private:
    unzFile      m_handle;
    unz_file_pos m_filePos;

public:
    ZipFile(void* handle, const ifilename& filename, const unz_file_info& fileInfo,
            const unz_file_pos& filePos);
    ~ZipFile();

private:
    virtual void doFillBuffer(weak< File::Ticket > ticket) const override;
    virtual void doWriteBuffer(weak< Ticket > ticket) const override;
};

}  // namespace BugEngine

/**************************************************************************************************/
#endif
