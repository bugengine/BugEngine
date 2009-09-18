/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_MESSAGE_HH_
#define BE_CORE_MESSAGE_HH_
/*****************************************************************************/

namespace BugEngine
{

class COREEXPORT message
{
private:
    char *m_msg;
public:
    message(const char *str);
    ~message();

private:
    message();
};

}


/*****************************************************************************/
#endif
