/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_PACKAGE_LOGGER_HH_
#define BE_PACKAGE_LOGGER_HH_
/**************************************************************************************************/
#include    <package/stdafx.h>

namespace BugEngine { namespace PackageBuilder
{

class be_api(PACKAGE) Logger
{
private:
    u32 m_errorCount;
private:
    enum LogLevel
    {
        Info,
        Warning,
        Error
    };
    void log(LogLevel level, const ifilename& file, const u32 line, const u32 begin, const u32 end,
             const char *message);
public:
    Logger();
    ~Logger();
public:
    void info(const ifilename& file, const u32 line, const u32 begin, const u32 end,
              const char* message);
    void warning(const ifilename& file, const u32 line, const u32 begin, const u32 end,
                 const char* message);
    void error(const ifilename& file, const u32 line, const u32 begin, const u32 end,
               const char* message);
    bool success() const;
};

}}

/**************************************************************************************************/
#endif
