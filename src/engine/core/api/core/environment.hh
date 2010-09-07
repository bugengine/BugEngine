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
    ipath   m_homeDirectory;
    ipath   m_dataDirectory;
    istring m_game;
    istring m_user;
public:
    static Environment& getEnvironment();

	void init(int argc, const char *argv[]);

    const ipath&    getHomeDirectory() const;
    const ipath&    getDataDirectory() const;

    const istring&  getGame() const;
    const istring&  getUser() const;
    size_t          getProcessorCount() const;
};

}

/*****************************************************************************/
#endif
