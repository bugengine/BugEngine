/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#ifndef BE_RTTI_AST_FILENAME_HH_
#define BE_RTTI_AST_FILENAME_HH_
/**************************************************************************************************/
#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/filesystem/file.script.hh>
#include <bugengine/rtti-ast/node/node.hh>

namespace BugEngine { namespace RTTI { namespace AST {

class be_api(RTTI_AST) FileName : public Node
{
private:
    const ifilename m_value;
    weak< File >    m_file;

protected:
    virtual bool resolve(DbContext & context) override;
    virtual bool isCompatible(const Type& expectedType) const override;
    virtual void doEval(const Type& expectedType, Value& result) const override;

public:
    FileName(const ifilename& value);
    ~FileName();
};

}}}  // namespace BugEngine::RTTI::AST

/**************************************************************************************************/
#endif
