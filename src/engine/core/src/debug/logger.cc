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
* Boston, MA                                                                  *
* 02110-1301  USA                                                             *
\*****************************************************************************/


#include    <core/stdafx.h>
#include    <core/log/logger.hh>
#include    <core/log/exception.hh>

#include    <minitl/ptr/scopedptr>


namespace BugEngine
{

EException::EException(const std::string& msg) :
    runtime_error(msg)
{
    Logger::root()->log(logFatal, "", 0, msg.c_str());
}




Logger::Logger() :
    m_listeners(),
    m_children(),
    m_name("root")
{
}



Logger::Logger(Logger*parent, const istring& name) :
    m_listeners(),
    m_children(),
    m_name(name)
{
    Assert(parent);
    parent->m_children.insert(std::make_pair( name, this ));
}

Logger::~Logger()
{
    for(size_t i = 0; i < m_listeners.size(); ++i)
        delete m_listeners[i];
    for(std::map<istring,Logger*>::const_iterator it = m_children.begin(); it != m_children.end(); ++it)
        delete it->second;
}

Logger* Logger::instance(const inamespace& name)
{
    Logger *result = root();

    for(size_t i = 0; i < name.size(); ++i)
    {
        std::map< istring, Logger* >::iterator it = result->m_children.find(name[i]);
        if(it == result->m_children.end())
            result = new Logger(result, name[i]);
        else
            result = it->second;
    }
    return result;
}

Logger* Logger::root()
{
    static Logger s_rootLogger;
    return &s_rootLogger;
}

bool Logger::log(const inamespace& name, eLogLevel level, const char *filename, int line, const char *msg)
{
    return instance(name)->log(level, filename, line, msg);
}

void Logger::addListener(ILogListener* listener)
{
    m_listeners.push_back(listener);
}

bool Logger::log(eLogLevel level, const char *filename, int line, const char *msg)
{
    bool result = false;
    for(std::vector< ILogListener* >::iterator it = m_listeners.begin(); it != m_listeners.end(); ++it)
    {
        result |= (*it)->log(m_name, level, filename, line, msg);
    }

    for(std::map< istring, Logger* >::iterator it = m_children.begin(); it != m_children.end(); ++it)
    {
        result |= it->second->log(level, filename, line, msg);
    }

    return result;

}

}
