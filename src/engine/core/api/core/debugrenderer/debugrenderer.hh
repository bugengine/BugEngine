/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

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
