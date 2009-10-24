/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_DEBUG_DWARF_HH_
#define BE_CORE_DEBUG_DWARF_HH_
/*****************************************************************************/
#include    <core/debug/symbols.hh>
#include    <elf.hh>

namespace BugEngine { namespace Debug
{

class DwarfModule : public Symbols::ISymbolResolver
{
private:
    struct AddressRange
    {
        u64 begin;
        u64 end;

        static bool consistent(AddressRange range1, AddressRange range2);
        bool operator<(AddressRange other) const;
        bool operator==(AddressRange other) const;
    };
    class CompilationUnit
    {
    public:
        const char*                                 name;
        AddressRange                                range;
        minitl::map<AddressRange, CompilationUnit>  children;

        CompilationUnit() : name(0) { }
        ~CompilationUnit() { }
    };
    template< Endianness e >
    class Buffer;
    typedef minitl::map<AddressRange, CompilationUnit>  UnitMap;
private:
    const u64       m_begin;
    const u64       m_end;
    const ifilename m_moduleName;
    UnitMap         m_units;
private:
    template< Endianness endianness >
    void parse(const Elf& elf);
public:
    DwarfModule(const ifilename& moduleName, const Elf& elf, u64 begin, u64 size);
    ~DwarfModule();

    virtual bool resolve(u64, BugEngine::Debug::Symbols::Symbol&) const override;
};

}}

/*****************************************************************************/
#endif

