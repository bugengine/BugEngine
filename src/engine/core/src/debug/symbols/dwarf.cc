/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>

#include    <dwarf.hh>

namespace BugEngine { namespace Debug
{

Dwarf::Dwarf(const ifilename& moduleName, const Elf& elf, u64 begin, u64 size)
:   m_begin(begin)
,   m_end(begin+size)
,   m_moduleName(moduleName)
{
}

Dwarf::~Dwarf()
{
}

bool Dwarf::resolve(u64 address, BugEngine::Debug::Symbols::Symbol& symbol) const override
{
    if(address >= m_begin && address < m_end)
    {
        strncpy(symbol.m_module, m_moduleName.str().c_str(), 4096);
        return true;
    }
    return false;
}

}}
