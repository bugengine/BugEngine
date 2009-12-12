/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_ENVIRONMENT_HH_
#define BE_CORE_ENVIRONMENT_HH_
/*****************************************************************************/

namespace BugEngine
{

class be_api(CORE) Environment
{
private:
    Environment();
    ~Environment();
private:
#ifndef BE_PLATFROM_CONSOLE
    ipath   m_homeDirectory;
    ipath   m_dataDirectory;
    ipath   m_pluginDirectory;
#endif
    istring m_game;
    istring m_user;
public:
    static const Environment& getEnvironment();

#ifndef BE_PLATFROM_CONSOLE
    const ipath&    getHomeDirectory() const;
    const ipath&    getDataDirectory() const;
    const ipath&    getPluginDirectory() const;
#endif
    const istring&  getGame() const;
    const istring&  getUser() const;
    size_t          getProcessorCount() const;
};

}

/*****************************************************************************/
#endif
