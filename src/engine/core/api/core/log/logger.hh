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

namespace BugEngine
{

enum eLogLevel
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
    virtual bool log(const istring& logname, eLogLevel level, const char *filename, int line, const char *msg) NOTHROW = 0;
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
    Logger(Logger* parent, const istring& name);
    ~Logger();

    static Logger* instance(const inamespace& name);
    static bool    log(const inamespace& name, eLogLevel level, const char *filename, int line, const char *msg);
    static Logger* root();

    void addListener(ILogListener* listener);
    bool log(eLogLevel level, const char *filename, int line, const char *msg);
private:
    Logger(const Logger& other);
    Logger& operator=(const Logger& other);
};

#ifdef _DEBUG
# define  be_spam(msg)        Logger::root()->log(logSpam, __FILE__,__LINE__,msg)
# define  be_debug(msg)       Logger::root()->log(logDebug, __FILE__,__LINE__,msg)
#else
# define  be_spam(msg)
# define  be_debug(msg)
#endif

#ifndef NDEBUG
# define  be_info(msg)        Logger::root()->log(logInfo, __FILE__,__LINE__,msg)
#else
# define  be_info(msg)
#endif

#define  be_warning(msg)      Logger::root()->log(logWarning, __FILE__,__LINE__,msg)
#define  be_error(msg)        Logger::root()->log(logError, __FILE__,__LINE__,msg)
#define  be_fatal(msg)        Logger::root()->log(logFatal, __FILE__,__LINE__,msg)

}

/*****************************************************************************/
#endif
