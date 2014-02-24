/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <package/stdafx.h>
#include    <package/logger.hh>

namespace BugEngine { namespace PackageBuilder
{

Logger::Logger()
    :   m_errorCount(0)
{
}

Logger::~Logger()
{
}

bool Logger::success() const
{
    return m_errorCount == 0;
}

void Logger::log(LogLevel level, const ifilename& filename, const u32 line, const u32 begin, const u32 end, const)
{

}

}}
