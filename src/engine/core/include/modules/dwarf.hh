/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_DEBUG_DWARF_HH_
#define BE_CORE_DEBUG_DWARF_HH_
/*****************************************************************************/
#include    <core/runtime/symbols.hh>
#include    <modules/elf.hh>

namespace BugEngine { namespace Runtime
{

namespace Dwarf
{
    class CompilationUnit;
    struct Abbreviation;
}

class DwarfModule : public SymbolResolver
{
private:
    class StringBuffer
    {
    private:
        const scopedptr<const StringBuffer> m_next;
        u64 const                           m_size;
        u64                                 m_current;
        char* const                         m_buffer;
    public:
        StringBuffer(char* buffer, u64 size, const StringBuffer* next = 0);
        StringBuffer(size_t size, const StringBuffer* next = 0);
        ~StringBuffer();

        const char* store(const char* string, size_t size);
    };
private:
    struct AddressRange
    {
        u64 begin;
        u64 end;

        static bool consistent(AddressRange range1, AddressRange range2);
        bool operator<(AddressRange other) const;
        bool operator==(AddressRange other) const;
    };
private:
    class CompilationUnit
    {
    public:
        const char*                                 name;
        AddressRange                                range;
        minitl::map<AddressRange, CompilationUnit>  children;

        CompilationUnit() : name(0) { }
        ~CompilationUnit() { }
    };
private:
    template< Endianness e > class Buffer;
    typedef minitl::map<AddressRange, CompilationUnit>  UnitMap;
private:
    const u64               m_begin;
    const u64               m_end;
    const ifilename         m_moduleName;
    UnitMap                 m_units;
    scopedptr<StringBuffer> m_strings;
    char*                   m_stringPool;
private:
    template< Endianness e >
    void parse(const Module& m);
    template< Endianness e >
    bool readInfos(Buffer<e>& buffer, UnitMap& units, const minitl::vector<Dwarf::Abbreviation>& abbreviations, u8 ptrSize);
    template< Endianness e >
    bool fillNode(Buffer<e>& buffer, CompilationUnit& unit, const Dwarf::Abbreviation& abbrev, const minitl::vector<Dwarf::Abbreviation>& abbreviations, u8 ptrSize);
    template< Endianness e >
    bool readAbbreviation(Buffer<e>& buffer, minitl::vector<Dwarf::Abbreviation>& abbreviations);
public:
    DwarfModule(const ifilename& moduleName, const Module& m, u64 begin, u64 size);
    ~DwarfModule();

    virtual bool resolve(u64 address, Symbol& symbol) const override;

    const char *storeString(const char *string);
    const char *indexedString(u64 offset) const;
};

}}

/*****************************************************************************/
#endif

