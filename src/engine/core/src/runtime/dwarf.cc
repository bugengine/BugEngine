/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>

#include    <modules/dwarf.hh>
#include    <modules/dwarftypes.hh>

namespace BugEngine { namespace Runtime
{

static const size_t c_stringBufferSize = 2048;

DwarfModule::StringBuffer::StringBuffer(size_t size, ref<const StringBuffer> next)
    :   m_next(next)
    ,   m_size(size)
    ,   m_current(0)
    ,   m_buffer(size)
{
}

DwarfModule::StringBuffer::~StringBuffer()
{
}

const char* DwarfModule::StringBuffer::store(const char* string, size_t size)
{
    if(m_size-m_current > size)
    {
        char* buffer = m_buffer+m_current;
        m_current += size + 1;
        strcpy(buffer, string);
        return buffer;
    }
    else
        return 0;
}

char* DwarfModule::StringBuffer::data()
{
    return m_buffer;
}

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
    friend class DwarfModule;
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

    void seek(u64 offset) { m_position = offset; be_assert(m_position < m_size, "seeking an invalid position : %d (size is %d)" | m_position | m_size); }
public:
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
    Integer<u32,endianness> m;
    memcpy(&m, m_buffer+m_position, 4);
    value.value = m;
    m_position += 4;
    if(value.value == 0xffffffff)
    {
        Integer<u64,endianness> m;
        memcpy(&m, m_buffer+m_position, 8);
        value.value = m;
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
        Integer<u64,endianness> m;
        memcpy(&m, m_buffer+m_position, 8);
        value.value = m;
        m_position += 8;
    }
    else
    {
        Integer<u32,endianness> m;
        memcpy(&m, m_buffer+m_position, 4);
        value.value = m;
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
    Integer<u32,endianness> m;
    memcpy(&m, m_buffer+m_position, 4);
    value = m;
    m_position += 4;
    return *this;
}

template< Endianness endianness >
DwarfModule::Buffer<endianness>& DwarfModule::Buffer<endianness>::operator>>(u16& value)
{
    Integer<u16,endianness> m;
    memcpy(&m, m_buffer+m_position, 2);
    value = m;
    m_position += 2;
    return *this;
}

template< Endianness endianness >
DwarfModule::Buffer<endianness>& DwarfModule::Buffer<endianness>::operator>>(u8& value)
{
    value = *(u8*)(m_buffer+m_position);
    m_position++;
    return *this;
}





DwarfModule::DwarfModule(const ifilename& moduleName, const Module& m, u64 begin, u64 size)
:   m_begin(begin)
,   m_end(begin+size)
,   m_moduleName(moduleName)
{
    be_info("loading module %s | %d-%d" | moduleName | begin | size);
    switch(m.endianness())
    {
    case Endianness_Little:
        be_debug("parsing littleendian");
        parse<Endianness_Little>(m);
        break;
    case Endianness_Big:
        be_debug("parsing bigendian");
        parse<Endianness_Big>(m);
        break;
    default:
        be_unimplemented();
    }
}

DwarfModule::~DwarfModule()
{
}

bool DwarfModule::resolve(u64 address, Symbol& symbol) const
{
    if(address >= m_begin && address < m_end)
    {
        AddressRange r; r.begin = r.end = address;
        UnitMap::const_iterator it = m_units.find(r);
        if(it != m_units.end())
        {
            fillSymbol(symbol, address, m_moduleName[m_moduleName.size()-1].c_str(), it->second.name, "", 0);
        }
        else
        {
            fillSymbol(symbol, address, m_moduleName[m_moduleName.size()-1].c_str(), "", "", 0);
        }
        return true;
    }
    return false;
}

template< Endianness endianness >
void DwarfModule::parse(const Module& module)
{
    u8* debugInfo = 0;
    u64 debugInfoSize = 0;
    u8* debugAbbrev = 0;
    u64 debugAbbrevSize = 0;
    u8* lineProgram = 0;

    const Module::Section& debug_str = module[".debug_str"];
    if(debug_str)
    {
        m_strings = ref<StringBuffer>::create<Arena::General>(be_checked_numcast<size_t>(debug_str.fileSize), m_strings);
        module.readSection(debug_str, m_strings->data());
    }
    const Module::Section& debug_info = module[".debug_info"];
    debugInfo = Memory<Arena::General>::Block<u8>(be_checked_numcast<size_t>(debug_info.fileSize));
    if(debug_info)
    {
        debugInfoSize = debug_info.size;
        module.readSection(debug_info, debugInfo);
    }
    const Module::Section& debug_abbrev = module[".debug_abbrev"];
    debugAbbrev = Memory<Arena::General>::Block<u8>(be_checked_numcast<size_t>(debug_abbrev.fileSize));
    if(debug_abbrev)
    {
        debugAbbrevSize = debug_abbrev.size;
        module.readSection(debug_abbrev, debugAbbrev);
    }
    const Module::Section& debug_line = module[".debug_line"];
    lineProgram = Memory<Arena::General>::Block<u8>(be_checked_numcast<size_t>(debug_line.fileSize));
    if(debug_line)
    {
        module.readSection(debug_line, lineProgram);
    }

    minitl::vector<Dwarf::Abbreviation, Arena::General> abbrev;
    Buffer<endianness> abbreviations(debugAbbrev, debugAbbrevSize);
    Buffer<endianness> info(debugInfo, debugInfoSize);

    while(info)
    {
        Dwarf::Unit unit;

        abbrev.resize(0);
        info >> unit.length;
        info >> unit.version;
        info >> unit.abbrev;
        info >> unit.ptrSize;

        abbreviations.seek(unit.abbrev);
        while(readAbbreviation(abbreviations, abbrev)) /*again*/;
        readInfos(info, m_units, abbrev, unit.ptrSize);
    }
}

const char * DwarfModule::storeString(const char *string)
{
    size_t size = strlen(string);
    const char *result = 0;
    be_assert(size < c_stringBufferSize, "string is too big to fit in a pool; string size is %d, pool size is %d" | size | c_stringBufferSize);
    if(!m_strings)
    {
        m_strings = ref<StringBuffer>::create<Arena::General>(c_stringBufferSize);
    }
    result = m_strings->store(string, size);
    if(!result)
    {
        m_strings = ref<StringBuffer>::create<Arena::General>(c_stringBufferSize, m_strings);
        result = m_strings->store(string, size);
        be_assert(result, "new empty pool could not store string");
    }
    return result;
}

const char * DwarfModule::indexedString(u64 offset) const
{
    return m_stringPool + offset;
}

template< Endianness endianness >
bool DwarfModule::readAbbreviation(Buffer<endianness>& buffer, minitl::vector<Dwarf::Abbreviation, Arena::General>& abbreviations)
{
    Dwarf::uleb128_t code;
    buffer >> code;
    if(code == 0)
        return false;

    if(abbreviations.size() < code)
        abbreviations.resize(be_checked_numcast<size_t>(code.value));
    Dwarf::Abbreviation& abbrev = abbreviations[be_checked_numcast<size_t>(code.value)-1];

    buffer >> abbrev.tag;
    buffer >> abbrev.children;
    abbrev.propertyCount = 0;
    do
    {
        buffer >> abbrev.properties[abbrev.propertyCount].attribute;
        buffer >> abbrev.properties[abbrev.propertyCount].type;
        if(abbrev.properties[abbrev.propertyCount].attribute == 0)
        {
            be_assert(abbrev.properties[abbrev.propertyCount].type == 0, "inconsistent entry with attribute %d and type %d" | abbrev.properties[abbrev.propertyCount].attribute | abbrev.properties[abbrev.propertyCount].type);
            return true;
        }
        abbrev.propertyCount++;
        be_assert(abbrev.propertyCount < Dwarf::Attribute_max, "too many attributes");
    } while(true);
}

template< Endianness endianness >
bool DwarfModule::fillNode(Buffer<endianness>& buffer, CompilationUnit& r, const Dwarf::Abbreviation& abbrev, const minitl::vector<Dwarf::Abbreviation, Arena::General>& abbreviations, u8 ptrSize)
{
    be_forceuse(r);
    unsigned attributesMatched = 0; 
    for(unsigned i = 0; i < abbrev.propertyCount; ++i)
    {
        u64 attribute = abbrev.properties[i].attribute;
        u64 type = abbrev.properties[i].type;
        if(type == Dwarf::Type_indirect)
        {
            Dwarf::uleb128_t indirectType;
            buffer >> indirectType;
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
                buffer.m_position += ptrSize;
                break;

            case Dwarf::Type_block2:
                {
                    u16 length;
                    buffer >> length;
                    buffer.m_position += length;
                }
                break;

            case Dwarf::Type_block4:
                {
                    u32 length;
                    buffer >> length;
                    buffer.m_position += length;
                }
                break;

            case Dwarf::Type_data2:
            case Dwarf::Type_ref2:
                buffer.m_position += 2;
                break;

            case Dwarf::Type_data4:
            case Dwarf::Type_ref4:
                buffer.m_position += 4;
                break;

            case Dwarf::Type_data8:
            case Dwarf::Type_ref8:
                buffer.m_position += 8;
                break;

            case Dwarf::Type_string:
                u8 c;
                do { buffer >> c; } while(c);
                break;

            case Dwarf::Type_block:
                {
                    Dwarf::uleb128_t length;
                    buffer >> length;
                    buffer.m_position += length;
                }
                break;

            case Dwarf::Type_block1:
                {
                    u8 length;
                    buffer >> length;
                    buffer.m_position += length;
                }
                break;

            case Dwarf::Type_data1:
            case Dwarf::Type_ref1:
                buffer.m_position ++;
                break;

            case Dwarf::Type_flag:
                buffer.m_position ++;
                break;

            case Dwarf::Type_sdata:
                {
                    Dwarf::uleb128_t value;
                    buffer >> value;
                }
                break;

            case Dwarf::Type_strp:
                {
                    Dwarf::offset_t size;
                    buffer >> size;
                }
                break;

            case Dwarf::Type_udata:
            case Dwarf::Type_ref_udata:
                {
                    Dwarf::uleb128_t value;
                    buffer >> value;
                }
                break;

            case Dwarf::Type_ref_addr:
                {
                    Dwarf::offset_t size;
                    buffer >> size;
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
        UnitMap m;
        while(readInfos(buffer, m, abbreviations, ptrSize)) /* Again */;
    }
    return false; //attributesMatched == 3;
}

template< Endianness endianness >
bool DwarfModule::readInfos(Buffer<endianness>& buffer, UnitMap& units, const minitl::vector<Dwarf::Abbreviation, Arena::General>& abbreviations, u8 ptrSize)
{
    Dwarf::uleb128_t l;

    buffer >> l;
    if(l == 0)
        return false;
    const Dwarf::Abbreviation& abbrev = abbreviations[be_checked_numcast<size_t>(l.value)-1];
    CompilationUnit u;
    if(fillNode(buffer, u, abbrev, abbreviations, ptrSize))
    {
        bool result = units.insert(std::make_pair(u.range, u)).second;
        (void)result;
        be_assert(result, "could not add unit %s because range %d-%d is already covered" | u.name | u.range.begin | u.range.end);
    }
    return true;
}





}}

