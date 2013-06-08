/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_FILESYSTEM_FILESYSTEMWATCH_HH_
#define BE_FILESYSTEM_FILESYSTEMWATCH_HH_
/*****************************************************************************/
#include    <filesystem/file.script.hh>
#include    <filesystem/folder.script.hh>

namespace BugEngine { namespace Watch
{

class FileSystemWatch
{
public:
    class Watch : public minitl::refcountable
    {

    };
    FileSystemWatch();
    ~FileSystemWatch();

    ref<Watch> watchDirectory(const ipath& folder);
    ref<Watch> watchFile(const ifilename& file);
};

}}

/*****************************************************************************/
#endif
