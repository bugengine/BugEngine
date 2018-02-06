/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <runtime/stdafx.h>

#include    <runtime/symbols.hh>

#include    <elf.hh>
#include    <pe.hh>
#include    <dwarf.hh>
#include    <pdb20.hh>
#include    <pdb70.hh>


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
    strncpy(symbol.m_module, module, sizeof(symbol.m_module)-1);
    symbol.m_module[sizeof(symbol.m_module)-1] = 0;
    strncpy(symbol.m_filename, filename, sizeof(symbol.m_filename)-1);
    symbol.m_filename[sizeof(symbol.m_filename)-1] = 0;
    strncpy(symbol.m_function, function, sizeof(symbol.m_function)-1);
    symbol.m_function[sizeof(symbol.m_function)-1] = 0;
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
            PE pe(infos.filename.str().name, 0);
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
            Elf elf(infos.filename.str().name, 0);
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
