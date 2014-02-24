/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_CORE_STRING_MESSAGE_HH_
#define BE_CORE_STRING_MESSAGE_HH_
/**************************************************************************************************/

namespace BugEngine
{

class be_api(CORE) message
{
private:
    const char *m_msg;
public:
    message(const char *str);
    ~message();

private:
    message();
};

}


/**************************************************************************************************/
#endif
