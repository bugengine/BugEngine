/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_DEBUG_LOG_HH_
#define BE_CORE_DEBUG_LOG_HH_
/*****************************************************************************/
#include <core/string/istring.hh>
#include <minitl/string/format.hh>
#include <minitl/ptr/refptr.hh>
#include <minitl/ptr/weakptr.hh>

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
    static be_api(CORE) const char* s_logNames[];
    virtual ~ILogListener() {}
    virtual bool log(const istring& logname, LogLevel level, const char *filename, int line, const char *msg) = 0;
};

class be_api(CORE) Logger : public minitl::refcountable
{
    friend class minitl::ref<Logger>;
private:
    minitl::vector< ILogListener* >                 m_listeners;
    minitl::hashmap< istring, minitl::ref<Logger> > m_children;
    minitl::weak<Logger>                            m_parent;
    istring                                         m_name;
private:
    Logger();
public:
    Logger(minitl::ref<Logger> parent, const istring& name);
    ~Logger();

    static minitl::ref<Logger> instance(const inamespace& name);
    static bool                log(const inamespace& name, LogLevel level, const char *filename, int line, const char *msg);
    static minitl::ref<Logger> root();

    void addListener(ILogListener* listener);
    bool log(LogLevel level, const char *filename, int line, const char *msg);
    template< size_t size >
    inline bool log(LogLevel level, const char *filename, int line, const minitl::format<size>& msg) { return log(level, filename, line, msg.c_str()); }
private:
    Logger(const Logger& other);
    Logger& operator=(const Logger& other);
};

#ifdef _DEBUG
# define  be_spam(msg)        Logger::root()->log(logSpam, __FILE__, __LINE__, (minitl::format<2048>)msg)
# define  be_debug(msg)       Logger::root()->log(logDebug, __FILE__, __LINE__, (minitl::format<2048>)msg)
#else
# define  be_spam(msg)
# define  be_debug(msg)
#endif

#ifndef NDEBUG
# define  be_info(msg)        Logger::root()->log(logInfo, __FILE__, __LINE__, (minitl::format<2048>)msg)
#else
# define  be_info(msg)
#endif

#define  be_warning(msg)      Logger::root()->log(logWarning, __FILE__, __LINE__, (minitl::format<2048>)msg)
#define  be_error(msg)        Logger::root()->log(logError, __FILE__, __LINE__, (minitl::format<2048>)msg)
#define  be_fatal(msg)        Logger::root()->log(logFatal, __FILE__, __LINE__, (minitl::format<2048>)msg)

}

/*****************************************************************************/
#endif
