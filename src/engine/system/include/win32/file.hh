/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_WIN32_FILE_HH_
#define BE_SYSTEM_WIN32_FILE_HH_
/*****************************************************************************/

namespace BugEngine
{

class Win32File : public File
{
private:
    ifilename m_file;
public:
    Win32File(ifilename file, File::Media media, u64 size);
    ~Win32File();
};

}

/*****************************************************************************/
#endif
