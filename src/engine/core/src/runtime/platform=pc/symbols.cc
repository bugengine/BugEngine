/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>

#include    <core/runtime/symbols.hh>

#include    <modules/elf.hh>
#include    <modules/pe.hh>
#include    <modules/dwarf.hh>
#include    <modules/pdb20.hh>
#include    <modules/pdb70.hh>


namespace BugEngine { namespace Runtime
{

SymbolResolver::SymbolResolver()
{
}

SymbolResolver::~SymbolResolver()
{
}

void SymbolResolver::fillSymbol(Symbol& symbol, u64 address, const char *module, const char *filename, const char *function, int line)
{
    strncpy(symbol.m_module, module, sizeof(symbol.m_module));
    strncpy(symbol.m_filename, filename, sizeof(symbol.m_filename));
    strncpy(symbol.m_function, function, sizeof(symbol.m_function));
    symbol.m_address = address;
    symbol.m_line = line;
}

ref<const SymbolResolver> SymbolResolver::loadSymbols(const SymbolInformations& infos, ref<const SymbolResolver> next)
{
    switch(infos.type)
    {
    case SymbolInformations::PDB70:
        // TODO
        return next;
    case SymbolInformations::PDB20:
        // TODO
        return next;
    case SymbolInformations::Coff:
        // TODO
        return next;
    case SymbolInformations::PEDwarf:
        {
            PE pe(infos.filename.str().c_str(), 0);
            if (pe)
            {
                ref<SymbolResolver> resolver = ref<DwarfModule>::create(Arena::debug(), infos.filename, pe, infos.offset, infos.size);
                resolver->m_next = next;
                return resolver;
            }
        }
        return next;
    case SymbolInformations::ELFDwarf:
        {
            Elf elf(infos.filename.str().c_str(), 0);
            if (elf)
            {
                ref<SymbolResolver> resolver = ref<DwarfModule>::create(Arena::debug(), infos.filename, elf, infos.offset, infos.size);
                resolver->m_next = next;
                return resolver;
            }
        }
        return next;
    case SymbolInformations::None:
        return next;
    default:
        be_warning("Unknown symbol type for symbols %s" | infos.filename);
        return next;
    }
}

bool SymbolResolver::resolve(Callstack::Address& address, Symbol& symbol) const
{
    fillSymbol(symbol, address.address(), "unknown module", "?", "?", 0);
    if (!resolve(address.address(), symbol))
    {
        return m_next ? m_next->resolve(address, symbol) : false;
    }
    else
        return true;
}

}}
