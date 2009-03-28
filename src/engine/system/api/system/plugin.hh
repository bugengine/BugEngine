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

#ifndef BE_SYSTEM_PLUGIN_HH_
#define BE_SYSTEM_PLUGIN_HH_
/*****************************************************************************/

namespace BugEngine
{


class SYSTEMEXPORT Plugin
{
private:
    typedef void* Handle;
    #ifdef WIN32
    # ifdef _WIN64
        typedef __int64 __rval;
    # else
        typedef int     __rval;
    # endif
    #else
        typedef int     __rval;
    #endif
    typedef __rval (* generic)(void);
private:
    Handle FPluginHandle;
private:
    generic _get(const std::string& name);
public:
    Plugin(const istring &pluginName);
    virtual ~Plugin(void);

    template< typename callback >
    callback get(const std::string& name)
    {
        return reinterpret_cast<callback>(_get(name));
    }

    operator const void*() const { return FPluginHandle; }
private:
    Plugin();
    Plugin(const Plugin& other);
    Plugin& operator=(const Plugin& other);
};

}

/*****************************************************************************/
#endif
