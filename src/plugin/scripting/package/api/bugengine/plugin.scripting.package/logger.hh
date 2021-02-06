/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_PACKAGE_LOGGER_HH_
#define BE_PACKAGE_LOGGER_HH_
/**************************************************************************************************/
#include <bugengine/plugin.scripting.package/stdafx.h>

namespace BugEngine { namespace PackageBuilder {

struct be_api(PACKAGE) Logger
{
    BE_NOCOPY(Logger);

private:
    u32 m_errorCount;

public:
    Logger();
    ~Logger();

public:
    void info(const ifilename& file, const u32 line, const char* message);
    void warning(const ifilename& file, const u32 line, const char* message);
    void error(const ifilename& file, const u32 line, const char* message);
    bool success() const;
};

}}  // namespace BugEngine::PackageBuilder

/**************************************************************************************************/
#endif
