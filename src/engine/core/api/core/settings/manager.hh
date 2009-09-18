/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_SETTINGS_MANAGER_H_
#define BE_CORE_SETTINGS_MANAGER_H_
/*****************************************************************************/

namespace BugEngine
{

class SettingsManager
{
public:
    static SettingsManager* getManager();
private:
    SettingsManager();
    ~SettingsManager();
};

}

/*****************************************************************************/
#endif
