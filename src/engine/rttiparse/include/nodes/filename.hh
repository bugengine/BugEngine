/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_RTTIPARSE_NODES_FILENAME_HH_
#define BE_RTTIPARSE_NODES_FILENAME_HH_
/**************************************************************************************************/
#include <bugengine/rttiparse/stdafx.h>
#include <bugengine/filesystem/file.script.hh>
#include <bugengine/rttiparse/valueparse.hh>

namespace BugEngine { namespace RTTI { namespace Parser {

class FileName : public Node
{
private:
    const ifilename m_value;
    weak< File >    m_file;

protected:
    virtual bool resolve(DbContext& context) override;
    virtual bool isCompatible(const Type& expectedType) const override;
    virtual void doEval(const Type& expectedType, Value& result) const override;

public:
    FileName(const ParseLocation& location, const ifilename& value);
    ~FileName();
};

}}}  // namespace BugEngine::RTTI::Parser

/**************************************************************************************************/
#endif
