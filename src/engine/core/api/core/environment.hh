/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_ENVIRONMENT_HH_
#define BE_CORE_ENVIRONMENT_HH_
/*****************************************************************************/

namespace BugEngine
{

class COREEXPORT Environment
{
private:
    Environment();
    ~Environment();
private:
    ipath   m_homeDirectory;
    ipath   m_dataDirectory;
    ipath   m_pluginDirectory;
    istring m_game;
    istring m_user;
public:
    static const Environment& getEnvironment();

    const ipath&    getHomeDirectory() const;
    const ipath&    getDataDirectory() const;
    const ipath&    getPluginDirectory() const;
    const istring&  getGame() const;
    const istring&  getUser() const;
    size_t          getProcessorCount() const;
};

}

/*****************************************************************************/
#endif
