/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_SCRIPT_HH_
#define BE_RTTI_SCRIPT_HH_
/*****************************************************************************/
#include    <core/memory/streams.hh>

namespace BugEngine
{

class Script
{
public:
    virtual ~Script() { }

    virtual void doFile(const ifilename& file) = 0;
    virtual void doFile(weak<AbstractMemoryStream> stream) = 0;
};

}

/*****************************************************************************/
#endif
