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

#ifndef BE_CORE_DEBUGRENDERER_HH_
#define BE_CORE_DEBUGRENDERER_HH_
/*****************************************************************************/
#include    <maths/vector.hh>
#include    <core/utils/singleton.hh>

namespace BugEngine
{

class COREEXPORT DebugRenderer : public Singleton<DebugRenderer>
{
    friend class Singleton<DebugRenderer>;
protected:
    DebugRenderer();
    virtual ~DebugRenderer() = 0;
    
public:
    enum
    {
        Line2D = 0,
        Line3D = 1,
        Quad2D = 2,
        Quad3D = 3,
        PrimitiveTypeCount = 4
    };
    virtual uint2 getScreenSize() const = 0;
    virtual uint2 getCharacterSize() const = 0;

    virtual void drawLine(int2 p1, int2 p2, color32 color) const = 0;
    virtual void drawQuad(int2 min, int2 max, color32 color) const = 0;
    virtual void drawRect(int2 min, int2 max, color32 color) const = 0;
    virtual void drawText(const char *text, int2 start, color32 color) const = 0;
};

}

/*****************************************************************************/
#endif
