/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

#ifndef BE_CORE_LOG_HH_
#define BE_CORE_LOG_HH_
/*****************************************************************************/
#include <core/string/istring.hh>
#include <minitl/string/format.hh>

namespace BugEngine
{

enum LogLevel
{
    logSpam = 0,
    logDebug = 1,
    logInfo = 2,
    logWarning = 3,
    logError = 4,
    logFatal = 5
};

class Logger;

class ILogListener
{
    friend class Logger;
protected:
    static COREEXPORT const char* s_logNames[];
    virtual ~ILogListener() {}
    virtual bool log(const istring& logname, LogLevel level, const char *filename, int line, const char *msg) NOTHROW = 0;
};

class COREEXPORT Logger
{
private:
    std::vector< ILogListener* >    m_listeners;
    std::map< istring, Logger* >    m_children;
    Logger*                         m_parent;
    istring                         m_name;
private:
    Logger();
public:
    class LogMessage : public minitl::format<2048>
    {
    private:
        Logger*     m_logger;
        LogLevel    m_level;
        const char *m_file;
        int         m_line;
    public:
        LogMessage(Logger* l, LogLevel level, const char *file, int line, const char *msg)
            :   minitl::format<2048>(msg)
            ,   m_logger(l)
            ,   m_level(level)
            ,   m_file(file)
            ,   m_line(line)
        {
        }
        ~LogMessage()
        {
            m_logger->log(m_level, m_file, m_line, this->c_str());
        }
    };
public:
    Logger(Logger* parent, const istring& name);
    ~Logger();

    static Logger* instance(const inamespace& name);
    static bool    log(const inamespace& name, LogLevel level, const char *filename, int line, const char *msg);
    static Logger* root();

    void addListener(ILogListener* listener);
    bool log(LogLevel level, const char *filename, int line, const char *msg);
private:
    Logger(const Logger& other);
    Logger& operator=(const Logger& other);
};

#ifdef _DEBUG
# define  be_spam(msg)        Logger::LogMessage(Logger::root(), logSpam, __FILE__, __LINE__, msg)
# define  be_debug(msg)       Logger::LogMessage(Logger::root(), logDebug, __FILE__, __LINE__, msg)
#else
# define  be_spam(msg)
# define  be_debug(msg)
#endif

#ifndef NDEBUG
# define  be_info(msg)        Logger::LogMessage(Logger::root(), logInfo, __FILE__, __LINE__, msg)
#else
# define  be_info(msg)
#endif

#define  be_warning(msg)      Logger::LogMessage(Logger::root(), logWarning, __FILE__, __LINE__, msg)
#define  be_error(msg)        Logger::LogMessage(Logger::root(), logError, __FILE__, __LINE__, msg)
#define  be_fatal(msg)        Logger::LogMessage(Logger::root(), logFatal, __FILE__, __LINE__, msg)

}

/*****************************************************************************/
#endif
