/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/rtti-ast/stdafx.h>
#include <bugengine/rtti-ast/node/filename.hh>

#include <bugengine/filesystem/file.script.hh>
#include <bugengine/rtti-ast/dbcontext.hh>

namespace BugEngine { namespace RTTI { namespace AST {

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

bool FileName::isCompatible(DbContext& context, const Type& expectedType) const
{
    if(!be_type< weak< const File > >().isA(expectedType))
    {
        context.error(this,
                      Message::MessageType("cannot cast file value to %s") | expectedType.name());
        return false;
    }
    else
        return true;
}

void FileName::doEval(const Type& expectedType, Value& result) const
{
    be_forceuse(expectedType);
    result = RTTI::Value(m_file);
}
}}}  // namespace BugEngine::RTTI::AST
