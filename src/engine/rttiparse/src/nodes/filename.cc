/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <rttiparse/stdafx.h>
#include    <nodes/filename.hh>
#include    <filesystem/file.script.hh>


namespace BugEngine { namespace RTTI { namespace Parser
{

FileName::FileName(const ParseLocation& location, const ifilename& value)
    :   Node(location)
    ,   m_value(value)
    ,   m_file()
{
    be_forceuse(m_value);
}

FileName::~FileName()
{
}

bool FileName::resolve(DbContext &context)
{
    m_file = context.rootFolder->openFile(m_value);
    if (!m_file)
    {
        context.error(m_location,
                      Message::MessageType("could not open file: %s: no such file or directory") | m_value);
    }
    return m_file;
}

bool FileName::isCompatible(const Type &expectedType) const
{
    return be_typeid< weak<const File> >::type().isA(expectedType);
}

void FileName::doEval(const Type& expectedType, Value& result) const
{
    be_assert(isCompatible(expectedType), "invalid conversion from %s to %s"
            | be_typeid< weak<const File> >::type()
            | expectedType);
    be_forceuse(expectedType);
    result = RTTI::Value(m_file);
}
}}}
