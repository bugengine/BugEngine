/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_LOGGER_HH_
#define BE_CORE_LOGGER_HH_
/**************************************************************************************************/
#include <bugengine/core/stdafx.h>
#include <bugengine/core/string/istring.hh>
#include <bugengine/minitl/hash_map.hh>
#include <bugengine/minitl/tuple.hh>
#include <bugengine/minitl/vector.hh>

namespace BugEngine {

enum LogLevel
{
    logSpam    = 0,
    logDebug   = 1,
    logInfo    = 2,
    logWarning = 3,
    logError   = 4,
    logFatal   = 5
};

class Logger;

class ILogListener : public minitl::refcountable
{
    friend class Logger;

protected:
    static be_api(CORE) const char* getLogLevelName(LogLevel level);
    virtual ~ILogListener()
    {
    }
    virtual bool log(const istring& logname, LogLevel level, const char* filename, int line,
                     const char* thread, const char* msg) const = 0;
};

class be_api(CORE) Logger : public minitl::refcountable
{
    friend class minitl::ref< Logger >;
    friend struct ScopedLogListener;
    BE_NOCOPY(Logger);

private:
    minitl::vector< minitl::weak< ILogListener > >    m_listeners;
    minitl::hashmap< istring, minitl::ref< Logger > > m_children;
    minitl::weak< Logger >                            m_parent;
    istring                                           m_name;

private:
    Logger();

public:
    Logger(minitl::ref< Logger > parent, const istring& name);
    ~Logger();

    static minitl::ref< Logger > instance(const inamespace& name);
    static bool log(const inamespace& name, LogLevel level, const char* filename, int line,
                    const char* msg);
    static minitl::ref< Logger > root();

    bool log(LogLevel level, const char* filename, int line, const char* msg) const;

private:
    void addListener(minitl::weak< ILogListener > listener);
    void removeListener(minitl::weak< ILogListener > listener);
    bool doLog(LogLevel level, istring logName, const char* filename, int line, const char* msg)
        const;
};

struct ScopedLogListener
{
private:
    minitl::scoped< ILogListener > const m_listener;

public:
    ScopedLogListener(minitl::scoped< ILogListener > listener) : m_listener(listener)
    {
        Logger::root()->addListener(m_listener);
    }
    ~ScopedLogListener()
    {
        Logger::root()->removeListener(m_listener);
    }

private:
    ScopedLogListener(const ScopedLogListener&);
    ScopedLogListener& operator=(const ScopedLogListener&);
};

#define ALLDEBUG
#if defined(_DEBUG) || defined(ALLDEBUG)
#    define be_spam(msg)                                                                           \
        ::BugEngine::Logger::root()->log(::BugEngine::logSpam, __FILE__, __LINE__,                 \
                                         (minitl::format< 1024u >)msg)
#    define be_debug(msg)                                                                          \
        ::BugEngine::Logger::root()->log(::BugEngine::logDebug, __FILE__, __LINE__,                \
                                         (minitl::format< 1024u >)msg)
#else
#    define be_spam(msg)
#    define be_debug(msg)
#endif

#if !defined(NDEBUG) || defined(ALLDEBUG)
#    define be_info(msg)                                                                           \
        ::BugEngine::Logger::root()->log(::BugEngine::logInfo, __FILE__, __LINE__,                 \
                                         (minitl::format< 1024u >)msg)
#else
#    define be_info(msg)
#endif

#define be_warning(msg)                                                                            \
    ::BugEngine::Logger::root()->log(::BugEngine::logWarning, __FILE__, __LINE__,                  \
                                     (minitl::format< 1024u >)msg)
#define be_error(msg)                                                                              \
    ::BugEngine::Logger::root()->log(::BugEngine::logError, __FILE__, __LINE__,                    \
                                     (minitl::format< 1024u >)msg)
#define be_fatal(msg)                                                                              \
    ::BugEngine::Logger::root()->log(::BugEngine::logFatal, __FILE__, __LINE__,                    \
                                     (minitl::format< 1024u >)msg)

}  // namespace BugEngine

/**************************************************************************************************/
#endif
