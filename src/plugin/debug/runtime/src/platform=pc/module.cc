/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/plugin.debug.runtime/stdafx.h>
#include <bugengine/plugin.debug.runtime/module.hh>
#include <bugengine/plugin.debug.runtime/symbols.hh>

#include <elf.hh>
#include <pe.hh>

namespace BugEngine { namespace Runtime {

Module::Module(const char* filename, u64 baseAddress)
    : m_filename(filename)
    , m_baseAddress(baseAddress)
    , m_sections(Arena::debug())
{
}

Module::~Module()
{
}

const Module::Section& Module::operator[](const istring& name) const
{
    static Section s_empty = {"", 0, 0, 0, 0};
    for(minitl::vector< Section >::const_iterator it = m_sections.begin(); it != m_sections.end();
        ++it)
    {
        if(it->name == name) return *it;
    }
    return s_empty;
}

void Module::readSection(const Section& /*section*/, void* /*data*/) const
{
    /*FILE*f = fopen(m_filename.str().name, "rb");
    fseek(f, be_checked_numcast<long int>(section.fileOffset), SEEK_SET);
    fread(data, be_checked_numcast<long int>(section.fileSize), 1, f);*/
}

}}  // namespace BugEngine::Runtime
