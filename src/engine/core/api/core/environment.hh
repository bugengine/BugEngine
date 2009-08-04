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
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

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
