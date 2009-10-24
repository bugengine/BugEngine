/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>

#include    <dwarf.hh>
#include    <dwarftypes.hh>

namespace BugEngine { namespace Debug
{

namespace Dwarf
{

struct size_t
{
    u64 value;
    operator u64() const { return value; }
};
struct offset_t
{
    u64 value;
    operator u64() const { return value; }
};
struct uleb128_t
{
    u64 value;
    operator u64() const { return value; }
};

struct Property
{
    uleb128_t   attribute;
    uleb128_t   type;
};

struct Abbreviation
{
    uleb128_t       tag;
    u8              children;
    unsigned int    propertyCount;
    Property        properties[Attribute_max];
};

struct Unit
{
    size_t      length;
    u16         version;
    offset_t    abbrev;
    u8          ptrSize;
};

}

bool DwarfModule::AddressRange::consistent(AddressRange range1, AddressRange range2)
{
    if(range1.begin > range1.end)
        return false;
    if(range2.begin > range2.end)
        return false;
    if(range1.begin <= range2.begin)
    {
        return range1.end <= range2.end;
    }
    else if(range1.begin <= range2.end)
    {
        return range1.end <= range2.end;
    }
    else
    {
        return range1.end > range2.end;
    }
}

bool DwarfModule::AddressRange::operator<(AddressRange other) const
{
    be_assert(consistent(*this, other) || consistent(other, *this), "inconsistent adress comparison : range [%d-%d] versus range [%d-%d]" | other.begin | other.end | begin | end);
    return end < other.begin;
}

bool DwarfModule::AddressRange::operator==(AddressRange other) const
{
    be_assert(consistent(*this, other) || consistent(other, *this), "inconsistent adress comparison : range [%d-%d] versus range [%d-%d]" | other.begin | other.end | begin | end);
    return (other.begin >= begin && other.begin <= end) || (begin >= other.begin && begin <= other.end);
}

template< Endianness endianness >
class DwarfModule::Buffer
{
private:
    u8*     m_buffer;
    u64     m_size;
    u64     m_position;
    bool    m_64;
public:
    Buffer(u8* buffer, u64 size);
    ~Buffer();

    operator const void*() const;
    bool operator !() const;

    void seek(u64 offset) { m_position = offset; be_assert(m_position < m_size, "seekking an invalid position : %d (size is %d)" | m_position | m_size); }

    void readUnit(Dwarf::Unit& unit);
    bool readInfos(UnitMap& units, const minitl::vector<Dwarf::Abbreviation>& abbreviations, u8 ptrSize);
    bool fillNode(CompilationUnit& unit, const Dwarf::Abbreviation& abbrev, const minitl::vector<Dwarf::Abbreviation>& abbreviations, u8 ptrSize);
    bool readAbbreviation(minitl::vector<Dwarf::Abbreviation>& abbreviations);
private:
    Buffer& operator>>(Dwarf::size_t& value);
    Buffer& operator>>(Dwarf::offset_t& value);
    Buffer& operator>>(Dwarf::uleb128_t& value);
    Buffer& operator>>(u32& value);
    Buffer& operator>>(u16& value);
    Buffer& operator>>(u8& value);
};

template< Endianness endianness >
DwarfModule::Buffer<endianness>::Buffer(u8* buffer, u64 size)
:   m_buffer(buffer)
,   m_size(size)
,   m_position(0)
,   m_64(false)
{
}

template< Endianness endianness >
DwarfModule::Buffer<endianness>::~Buffer()
{
}

template< Endianness endianness >
DwarfModule::Buffer<endianness>::operator const void*() const
{
    return (const void*)(m_size-m_position);
}

template< Endianness endianness >
bool DwarfModule::Buffer<endianness>::operator !() const
{
    return m_position >= m_size;
}

template< Endianness endianness >
DwarfModule::Buffer<endianness>& DwarfModule::Buffer<endianness>::operator>>(Dwarf::size_t& value)
{
    value.value = *(Integer<u32,endianness>*)(m_buffer+m_position);
    m_position += 4;
    if(value.value == 0xffffffff)
    {
        value.value = *(Integer<u64,endianness>*)(m_buffer+m_position);
        m_position += 8;
        m_64 = true;
    }
    return *this;
}

template< Endianness endianness >
DwarfModule::Buffer<endianness>& DwarfModule::Buffer<endianness>::operator>>(Dwarf::offset_t& value)
{
    if(m_64)
    {
        value.value = *(Integer<u64,endianness>*)(m_buffer+m_position);
        m_position += 8;
    }
    else
    {
        value.value = *(Integer<u32,endianness>*)(m_buffer+m_position);
        m_position += 4;
    }
    return *this;
}

template< Endianness endianness >
DwarfModule::Buffer<endianness>& DwarfModule::Buffer<endianness>::operator>>(Dwarf::uleb128_t& value)
{
    int i = 0;
    value.value = 0;
    do
    {
        value.value += ((*(m_buffer+m_position)) & 0x7f) << i;
        i+=7;
        m_position++;
    } while((*(m_buffer+m_position-1)) & 0x80);
    return *this;
}

    template< Endianness endianness >
DwarfModule::Buffer<endianness>& DwarfModule::Buffer<endianness>::operator>>(u32& value)
{
    value = *(Integer<u32,endianness>*)(m_buffer+m_position);
    m_position+=4;
    return *this;
}

template< Endianness endianness >
DwarfModule::Buffer<endianness>& DwarfModule::Buffer<endianness>::operator>>(u16& value)
{
    value = *(Integer<u16,endianness>*)(m_buffer+m_position);
    m_position+=2;
    return *this;
}

template< Endianness endianness >
DwarfModule::Buffer<endianness>& DwarfModule::Buffer<endianness>::operator>>(u8& value)
{
    value = *(u8*)(m_buffer+m_position);
    m_position++;
    return *this;
}

template< Endianness endianness >
bool DwarfModule::Buffer<endianness>::fillNode(CompilationUnit& r, const Dwarf::Abbreviation& abbrev, const minitl::vector<Dwarf::Abbreviation>& abbreviations, u8 ptrSize)
{
    unsigned attributesMatched = 0; 
    for(unsigned i = 0; i < abbrev.propertyCount; ++i)
    {
        u64 attribute = abbrev.properties[i].attribute;
        u64 type = abbrev.properties[i].type;
        if(type == Dwarf::Type_indirect)
        {
            Dwarf::uleb128_t indirectType;
            *this >> indirectType;
            type = indirectType;
        }

        if(attribute == Dwarf::Attribute_name)
        {
            attributesMatched++;
        }
        else if(attribute == Dwarf::Attribute_low_pc)
        {
            attributesMatched++;
        }
        else if(attribute == Dwarf::Attribute_high_pc)
        {
            attributesMatched++;
        }

        switch(type)
        {
            case Dwarf::Type_addr:
                m_position += ptrSize;
                break;

            case Dwarf::Type_block2:
                {
                    u16 length;
                    *this >> length;
                    m_position += length;
                }
                break;

            case Dwarf::Type_block4:
                {
                    u32 length;
                    *this >> length;
                    m_position += length;
                }
                break;

            case Dwarf::Type_data2:
            case Dwarf::Type_ref2:
                m_position += 2;
                break;

            case Dwarf::Type_data4:
            case Dwarf::Type_ref4:
                m_position += 4;
                break;

            case Dwarf::Type_data8:
            case Dwarf::Type_ref8:
                m_position += 8;
                break;

            case Dwarf::Type_string:
                u8 c;
                do { *this >> c; } while(c);
                break;

            case Dwarf::Type_block:
                {
                    Dwarf::uleb128_t length;
                    *this >> length;
                    m_position += length;
                }
                break;

            case Dwarf::Type_block1:
                {
                    u8 length;
                    *this >> length;
                    m_position += length;
                }
                break;

            case Dwarf::Type_data1:
            case Dwarf::Type_ref1:
                m_position ++;
                break;

            case Dwarf::Type_flag:
                m_position ++;
                break;

            case Dwarf::Type_sdata:
                {
                    Dwarf::uleb128_t value;
                    *this >> value;
                }
                break;

            case Dwarf::Type_strp:
                {
                    Dwarf::offset_t size;
                    *this >> size;
                }
                break;

            case Dwarf::Type_udata:
            case Dwarf::Type_ref_udata:
                {
                    Dwarf::uleb128_t value;
                    *this >> value;
                }
                break;

            case Dwarf::Type_ref_addr:
                {
                    Dwarf::offset_t size;
                    *this >> size;
                }
                break;

            case Dwarf::Type_indirect:
                be_unimplemented();
                break;

            default:
                be_unimplemented();
                break;
        };
    }
    if(abbrev.children)
    {
        while(readInfos(r.children, abbreviations, ptrSize)) /* Again */;
    }
    return attributesMatched == 3;
}

template< Endianness endianness >
bool DwarfModule::Buffer<endianness>::readInfos(UnitMap& units, const minitl::vector<Dwarf::Abbreviation>& abbreviations, u8 ptrSize)
{
    Dwarf::uleb128_t l;

    *this >> l;
    if(l == 0)
        return false;
    const Dwarf::Abbreviation& abbrev = abbreviations[checked_numcast<size_t>(l.value)-1];
    CompilationUnit u;
    if(fillNode(u, abbrev, abbreviations, ptrSize))
    {
        bool result = units.insert(std::make_pair(u.range, u)).second;
        be_assert(result, "could not add unit %s because range %d-%d is already covered" | u.name | u.range.begin | u.range.end);
    }
    return true;
}

template< Endianness endianness >
void DwarfModule::Buffer<endianness>::readUnit(Dwarf::Unit& unit)
{
    *this >> unit.length;
    *this >> unit.version;
    *this >> unit.abbrev;
    *this >> unit.ptrSize;
}

template< Endianness endianness >
bool DwarfModule::Buffer<endianness>::readAbbreviation(minitl::vector<Dwarf::Abbreviation>& abbreviations)
{
    Dwarf::uleb128_t code;
    *this >> code;
    if(code == 0)
        return false;

    if(abbreviations.size() < code)
        abbreviations.resize(code);
    Dwarf::Abbreviation& abbrev = abbreviations[code-1];

    *this >> abbrev.tag;
    *this >> abbrev.children;
    abbrev.propertyCount = 0;
    do
    {
        *this >> abbrev.properties[abbrev.propertyCount].attribute;
        *this >> abbrev.properties[abbrev.propertyCount].type;
        if(abbrev.properties[abbrev.propertyCount].attribute == 0)
        {
            be_assert(abbrev.properties[abbrev.propertyCount].type == 0, "inconsistent entry with attribute %d and type %d" | abbrev.properties[abbrev.propertyCount].attribute | abbrev.properties[abbrev.propertyCount].type);
            return true;
        }
        abbrev.propertyCount++;
        be_assert(abbrev.propertyCount < Dwarf::Attribute_max, "too many attributes");
    } while(true);
}





DwarfModule::DwarfModule(const ifilename& moduleName, const Elf& elf, u64 begin, u64 size)
:   m_begin(begin)
,   m_end(begin+size)
,   m_moduleName(moduleName)
{
    switch(elf.endianness())
    {
    case msb_littleendian:
        be_debug("parsing littleendian");
        parse<Endianness_Little>(elf);
        break;
    case msb_bigendian:
        be_debug("parsing bigendian");
        parse<Endianness_Big>(elf);
        break;
    default:
        be_unimplemented();
    }
}

DwarfModule::~DwarfModule()
{
}

bool DwarfModule::resolve(u64 address, BugEngine::Debug::Symbols::Symbol& symbol) const override
{
    if(address >= m_begin && address < m_end)
    {
        strncpy(symbol.m_module, m_moduleName.str().c_str(), sizeof(symbol.m_module));
        AddressRange r; r.begin = r.end = address;
        UnitMap::const_iterator it = m_units.find(r);
        if(it != m_units.end())
        {
            strncpy(symbol.m_filename, it->second.name, sizeof(symbol.m_filename));
        }
        return true;
    }
    return false;
}

template< Endianness endianness >
void DwarfModule::parse(const Elf& elf)
{
    u8* debugInfo = 0;
    u64 debugInfoSize = 0;
    u8* debugAbbrev = 0;
    u64 debugAbbrevSize = 0;
    u8* lineProgram = 0;

    for (const Elf::Section* s = elf.begin(); s != elf.end(); ++s)
    {
        if(strcmp(s->name, ".debug_info") == 0)
        {
            debugInfo = (u8*)be_malloc(s->fileSize);
            debugInfoSize = s->fileSize;
            elf.readSection(s, debugInfo);
        }
        if(strcmp(s->name, ".debug_abbrev") == 0)
        {
            debugAbbrev = (u8*)be_malloc(s->fileSize);
            debugAbbrevSize = s->fileSize;
            elf.readSection(s, debugAbbrev);
        }
        if(strcmp(s->name, ".debug_line") == 0)
        {
            lineProgram = (u8*)be_malloc(s->fileSize);
            elf.readSection(s, lineProgram);
        }
    }

    minitl::vector<Dwarf::Abbreviation> abbrev;
    Buffer<endianness> abbreviations(debugAbbrev, debugAbbrevSize);
    Buffer<endianness> info(debugInfo, debugInfoSize);

    while(info)
    {
        Dwarf::Unit u;
        abbrev.resize(0);
        info.readUnit(u);
        abbreviations.seek(u.abbrev);
        while(abbreviations.readAbbreviation(abbrev)) /*again*/;
        info.readInfos(m_units, abbrev, u.ptrSize);
    }

    be_free(lineProgram);
    be_free(debugAbbrev);
    be_free(debugInfo);
}

}}

