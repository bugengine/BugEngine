/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/introspect/stdafx.h>
#include <bugengine/introspect/node/filename.hh>

#include <bugengine/filesystem/file.script.hh>
#include <bugengine/introspect/dbcontext.hh>

namespace BugEngine { namespace Meta { namespace AST {

FileName::FileName(const ifilename& value) : Node(), m_value(value), m_file()
{
    be_forceuse(m_value);
}

FileName::~FileName()
{
}

ConversionCost FileName::distance(const Type& type) const
{
    return ConversionCalculator< weak< const File > >::calculate(type);
}

bool FileName::doResolve(DbContext& context)
{
    m_file = context.rootFolder->openFile(m_value);
    if(!m_file)
    {
        context.error(this,
                      Message::MessageType("could not open file: %s: no such file or directory")
                          | m_value);
        return false;
    }
    return true;
}

void FileName::doEval(const Type& expectedType, Value& result) const
{
    be_forceuse(expectedType);
    result = Meta::Value(m_file);
}

void FileName::doVisit(Node::Visitor& visitor) const
{
    visitor.accept(this);
}

}}}  // namespace BugEngine::Meta::AST
