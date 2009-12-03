/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */


#include    <core/stdafx.h>
#include    <core/debug/logger.hh>
#include    <core/debug/exception.hh>

#include    <minitl/ptr/scopedptr.hh>


namespace BugEngine
{

EException::EException(const std::string& msg) :
    runtime_error(msg)
{
    Logger::root()->log(logFatal, "", 0, msg.c_str());
}

const char* ILogListener::s_logNames[] =
{
    " SPAM  ",
    " DEBUG ",
    " INFO  ",
    "WARNING",
    " ERROR ",
    " FATAL "
};


Logger::Logger() :
    m_listeners(),
    m_children(),
    m_name("")
{
}



Logger::Logger(ref<Logger> parent, const istring& name)
    :   m_listeners()
    ,   m_children()
    ,   m_parent(parent)
    ,   m_name(name)
{
}

Logger::~Logger()
{
    for(size_t i = 0; i < m_listeners.size(); ++i)
        delete m_listeners[i];
}

ref<Logger> Logger::instance(const inamespace& name)
{
    ref<Logger> result = root();

    for(size_t i = 0; i < name.size(); ++i)
    {
        std::map< istring, ref<Logger> >::iterator it = result->m_children.find(name[i]);
        if(it == result->m_children.end())
        {
            ref<Logger> next = ref<Logger>::create(result, name[i]);
            result->m_children.insert(std::make_pair(name[i], next));
            result = next;
        }
        else
            result = it->second;
    }
    return result;
}

ref<Logger> Logger::root()
{
    static ref<Logger> s_rootLogger = ref<Logger>::create();
    return s_rootLogger;
}

bool Logger::log(const inamespace& name, LogLevel level, const char *filename, int line, const char *msg)
{
    return instance(name)->log(level, filename, line, msg);
}

void Logger::addListener(ILogListener* listener)
{
    m_listeners.push_back(listener);
}

bool Logger::log(LogLevel level, const char *filename, int line, const char *msg)
{
    bool result = false;
    for(std::vector< ILogListener* >::iterator it = m_listeners.begin(); it != m_listeners.end(); ++it)
    {
        result |= (*it)->log(m_name, level, filename, line, msg);
    }

    if(m_parent)
    {
        result |= m_parent->log(level, filename, line, msg);
    }

    return result;

}

}
