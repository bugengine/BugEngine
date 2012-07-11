/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_DEBUG_LOGGER_HH_
#define BE_CORE_DEBUG_LOGGER_HH_
/*****************************************************************************/
#include <core/string/istring.hh>
#include <minitl/string/format.hh>
#include <minitl/ptr/refptr.hh>
#include <minitl/ptr/scopedptr.hh>
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

class ILogListener : public minitl::refcountable
{
    friend class Logger;
protected:
    static be_api(CORE) const char* s_logNames[];
    virtual ~ILogListener() {}
    virtual bool log(const istring& logname, LogLevel level, const char *filename, int line, const char *msg) const = 0;
};

class be_api(CORE) Logger : public minitl::refcountable
{
    friend class minitl::ref<Logger>;
    friend struct ScopedLogListener;
    BE_NOCOPY(Logger);
private:
    minitl::vector< minitl::weak<ILogListener> >    m_listeners;
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

    bool log(LogLevel level, const char *filename, int line, const char *msg) const;
    template< size_t size >
    inline bool log(LogLevel level, const char *filename, int line, const minitl::format<size>& msg) const { return log(level, filename, line, msg.c_str()); }
private:
    void addListener(minitl::weak<ILogListener> listener);
    void removeListener(minitl::weak<ILogListener> listener);
};

struct ScopedLogListener
{
private:
    minitl::scoped<ILogListener> const m_listener;
public:
    ScopedLogListener(minitl::scoped<ILogListener> listener)
        :   m_listener(listener)
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
# define  be_spam(msg)        ::BugEngine::Logger::root()->log(::BugEngine::logSpam, __FILE__, __LINE__, (minitl::format<2048>)msg)
# define  be_debug(msg)       ::BugEngine::Logger::root()->log(::BugEngine::logDebug, __FILE__, __LINE__, (minitl::format<2048>)msg)
#else
# define  be_spam(msg)
# define  be_debug(msg)
#endif

#if !defined(NDEBUG) || defined(ALLDEBUG)
# define  be_info(msg)        ::BugEngine::Logger::root()->log(::BugEngine::logInfo, __FILE__, __LINE__, (minitl::format<2048>)msg)
#else
# define  be_info(msg)
#endif

#define  be_warning(msg)      ::BugEngine::Logger::root()->log(::BugEngine::logWarning, __FILE__, __LINE__, (minitl::format<2048>)msg)
#define  be_error(msg)        ::BugEngine::Logger::root()->log(::BugEngine::logError, __FILE__, __LINE__, (minitl::format<2048>)msg)
#define  be_fatal(msg)        ::BugEngine::Logger::root()->log(::BugEngine::logFatal, __FILE__, __LINE__, (minitl::format<2048>)msg)

}

/*****************************************************************************/
#endif
