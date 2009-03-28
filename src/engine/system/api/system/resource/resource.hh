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

#ifndef BE_SYSTEM_RESOURCE_RESOURCE_HH_
#define BE_SYSTEM_RESOURCE_RESOURCE_HH_
/*****************************************************************************/

namespace BugEngine
{

class MemoryStream;
class ifilename;

template< typename Type, typename Pipeline = typename Type::Pipeline >
class Resource
{
private:
    Type*       m_resource;
    Pipeline*   m_loader;
protected:
    void load(Pipeline* loader, const ifilename& file);
    void unload();
public:
    Resource();
    Resource(Pipeline* loader, const ifilename& file);
    ~Resource();

    inline bool isBound() const;

    inline Type* get();

    inline operator void*() const;
    inline Type* operator->();
    inline const Type* operator->() const;
    inline Type& operator*();
    inline const Type& operator*() const;
};

}

#include "resource.inl"

/*****************************************************************************/
#endif
