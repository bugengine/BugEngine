/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/core/stdafx.h>
#include <bugengine/core/logger.hh>

#include <bugengine/core/threads/thread.hh>
#include <bugengine/minitl/scopedptr.hh>

namespace BugEngine {

static const char* s_logLevelNames[]
    = {" SPAM  ", " DEBUG ", " INFO  ", "WARNING", " ERROR ", " FATAL "};

be_api(CORE) const char* ILogListener::getLogLevelName(LogLevel level)
{
    if(u32(level) <= logFatal)
        return s_logLevelNames[level];
    else
        return "Unknown";
}

Logger::Logger() : m_listeners(Arena::debug()), m_children(Arena::debug()), m_name("all")
{
}

Logger::Logger(ref< Logger > parent, const istring& name)
    : m_listeners(Arena::debug())
    , m_children(Arena::debug())
    , m_parent(parent)
    , m_name(name)
{
}

Logger::~Logger()
{
}

ref< Logger > Logger::instance(const inamespace& name)
{
    ref< Logger > result = root();

    for(u32 i = 0; i < name.size(); ++i)
    {
        minitl::hashmap< istring, ref< Logger > >::iterator it = result->m_children.find(name[i]);
        if(it == result->m_children.end())
        {
            ref< Logger > next = ref< Logger >::create(Arena::debug(), result, name[i]);
            result->m_children.insert(name[i], next);
            result = next;
        }
        else
            result = it->second;
    }
    return result;
}

ref< Logger > Logger::root()
{
    static ref< Logger > s_rootLogger = ref< Logger >::create(Arena::debug());
    return s_rootLogger;
}

bool Logger::log(const inamespace& name, LogLevel level, const char* filename, int line,
                 const char* msg)
{
    return instance(name)->log(level, filename, line, msg);
}

void Logger::addListener(weak< ILogListener > listener)
{
    m_listeners.push_back(listener);
}

void Logger::removeListener(weak< ILogListener > listener)
{
    for(minitl::vector< weak< ILogListener > >::iterator it = m_listeners.begin();
        it != m_listeners.end(); ++it)
    {
        if(*it == listener)
        {
            m_listeners.erase(it);
            return;
        }
    }
    be_warning("unable to remove listener");
}

bool Logger::log(LogLevel level, const char* filename, int line, const char* msg) const
{
    return doLog(level, m_name, filename, line, msg);
}

bool Logger::doLog(LogLevel level, istring logName, const char* filename, int line,
                   const char* msg) const
{
    bool result = false;
    for(minitl::vector< weak< ILogListener > >::const_iterator it = m_listeners.begin();
        it != m_listeners.end(); ++it)
    {
        result |= (*it)->log(logName, level, filename, line, Thread::name().c_str(), msg);
    }

    if(m_parent)
    {
        result |= m_parent->doLog(level, logName, filename, line, msg);
    }

    return result;
}

}  // namespace BugEngine
