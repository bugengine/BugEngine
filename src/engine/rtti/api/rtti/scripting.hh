/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_SCRIPTING_H_
#define BE_RTTI_SCRIPTING_H_
/*****************************************************************************/
#include    <rtti/scripting.hh>
#include    <system/filesystem.hh>

namespace BugEngine
{

class Scripting
{
public:
    virtual void doFile(const ifilename& file) = 0;
    virtual void doFile(weak<IMemoryStream> stream, const char *filename = 0) = 0;
    virtual ~Scripting() { }
};

}

/*****************************************************************************/
#endif
