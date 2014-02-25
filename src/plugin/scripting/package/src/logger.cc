/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <package/stdafx.h>
#include    <package/logger.hh>
#include    <core/logger.hh>

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

void Logger::info(const ifilename& file, const u32 line, const char* message)
{
    ::BugEngine::Logger::root()->log(::BugEngine::logInfo, file.str().name, line, message);
}

void Logger::warning(const ifilename& file, const u32 line, const char* message)
{
    ::BugEngine::Logger::root()->log(::BugEngine::logWarning, file.str().name, line, message);
}

void Logger::error(const ifilename& file, const u32 line, const char* message)
{
    ::BugEngine::Logger::root()->log(::BugEngine::logError, file.str().name, line, message);
    m_errorCount ++;
}

}}
