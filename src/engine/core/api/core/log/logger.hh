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

namespace BugEngine
{

enum eLogLevel
{
    logSpam,
    logAssert,
    logDebug,
    logInfo,
    logWarning,
    logError,
    logFatal
};

class Logger;

class ILogListener
{
    friend class Logger;
protected:
    virtual ~ILogListener() {}
    virtual bool log(const istring& logname, eLogLevel level, const char *filename, int line, const char *msg) NOTHROW = 0;
};

class COREEXPORT Logger
{
private:
    std::vector< ILogListener* >    m_listeners;
    std::map< istring, Logger* >    m_children;
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
# define  lspam(msg)        log(logSpam, __FILE__,__LINE__,msg)
# define _lspam(ns,msg)     BugEngine::Logger::instance(ns)->lspam(msg)
# define  ldebug(msg)       log(logDebug, __FILE__,__LINE__,msg)
# define _ldebug(ns,msg)    BugEngine::Logger::instance(ns)->ldebug(msg)
#else
# define  lspam(msg)
# define _lspam(ns,msg)
# define  ldebug(msg)
# define _ldebug(ns,msg)
#endif

#ifndef NDEBUG
# define  linfo(msg)        log(logInfo, __FILE__,__LINE__,msg)
# define _linfo(ns,msg)     BugEngine::Logger::instance(ns)->lspam(msg)
#else
# define  linfo(msg)
# define _linfo(ns,msg)
#endif

#define  lwarning(msg)      log(logWarning, __FILE__,__LINE__,msg)
#define _lwarning(ns,msg)   BugEngine::Logger::instance(ns)->lwarning(msg)
#define  lerror(msg)        log(logError, __FILE__,__LINE__,msg)
#define _lerror(ns,msg)     BugEngine::Logger::instance(ns)->lerror(msg)
#define  lfatal(msg)        log(logFatal, __FILE__,__LINE__,msg)
#define _lfatal(ns,msg)     BugEngine::Logger::instance(ns)->lfatal(msg)

}

/*****************************************************************************/
#endif
