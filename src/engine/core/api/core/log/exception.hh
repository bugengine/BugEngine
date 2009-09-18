/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef _CORE_EXCEPTION_H_
#define _CORE_EXCEPTION_H_
/*****************************************************************************/
#include    <stdexcept>

namespace BugEngine
{

class COREEXPORT EException : public std::runtime_error
{
public:
    EException(const std::string& msg);
};

}

/*****************************************************************************/
#endif
