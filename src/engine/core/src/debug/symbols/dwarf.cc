/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>

#include    <dwarf.hh>
#include    <dwarftypes.hh>

namespace BugEngine { namespace Debug {
{

namespace Dwarf
{

struct CompilationUnit
{
public:
    const char*         m_name;
    u64                 m_begin;
    u64                 m_end;
    void*               m_lineInfo;
    CompilationUnit*    m_left;
    CompilationUnit*    m_right;
public:
    CompilationUnit();
    ~CompilationUnit();
};

struct Function
{
private:
    const char *        m_name;
    u64                 m_begin;
    u64                 m_end;
    Function*           m_left;
    Function*           m_right;
};

struct Header
{
public:
    u64     size;
    u16     version;
    u64     debugAbbrevOffset;
    u8      addressSize;
};

struct InformationEntry
{
public:
    u64             tag;
    u64             debugAbbrevOffset;
    DwarfAttribute  attributes[16];
};

template< Endianness endianness >
struct Buffer
{
private:
    u8*     m_buffer;
    u64     m_size;
    u64     m_position;
    bool    m_64;
public:
    struct dwsize_t
    {
        u64 value;
        operator u64() const { return value; }
    };
    struct dwoffset_t
    {
        u64 value;
        operator u64() const { return value; }
    };
    struct dwuleb128_t
    {
        u64 value;
        operator u64() const { return value; }
    };
public:
    DwarfBuffer(u8* buffer, u64 size);
    ~DwarfBuffer();

    operator const void*() const;

    DwarfBuffer& operator>>(DwarfHeader& header);
private:
    DwarfBuffer& operator>>(dwsize_t& value);
    DwarfBuffer& operator>>(dwoffset_t& value);
    DwarfBuffer& operator>>(dwuleb128_t& value);
    DwarfBuffer& operator>>(u16& value);
    DwarfBuffer& operator>>(u8& value);
};

}

template< Endianness endianness >
DwarfBuffer<endianness>::DwarfBuffer(u8* buffer, u64 size)
:   m_buffer(buffer)
,   m_size(size)
,   m_position(0)
,   m_64(false)
{
}

template< Endianness endianness >
DwarfBuffer<endianness>::~DwarfBuffer()
{
}

template< Endianness endianness >
DwarfBuffer<endianness>::operator const void*() const
{
    return (const void*)m_size-m_position;
}

template< Endianness endianness >
DwarfBuffer<endianness>& DwarfBuffer<endianness>::operator>>(DwarfHeader& header)
{
    dwsize_t size;
    *this >> size;
    header.size = size;

    *this >> header.version;

    dwoffset_t offset;
    *this >> offset;
    header.debugAbbrevOffset = offset;

    *this >> header.addressSize;

    return *this;
}

template< Endianness endianness >
DwarfBuffer<endianness>& DwarfBuffer<endianness>::operator>>(dwsize_t& value)
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
DwarfBuffer<endianness>& DwarfBuffer<endianness>::operator>>(dwoffset_t& value)
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
DwarfBuffer<endianness>& DwarfBuffer<endianness>::operator>>(dwuleb128_t& value)
{
    int i = 0;
    do
    {
        value.value += ((*(m_buffer+m_position)) & 0x7f) << i;
        i+=7;
        m_position++;
    } while((*(m_buffer+m_position-1)) & 0x80);
    return *this;
}

template< Endianness endianness >
DwarfBuffer<endianness>& DwarfBuffer<endianness>::operator>>(u16& value)
{
    value = *(Integer<u16,endianness>*)(m_buffer+m_position);
    m_position+=2;
    return *this;
}

template< Endianness endianness >
DwarfBuffer<endianness>& DwarfBuffer<endianness>::operator>>(u8& value)
{
    value = *(Integer<u8,endianness>*)(m_buffer+m_position);
    m_position++;
    return *this;
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
        strncpy(symbol.m_module, m_moduleName.str().c_str(), 4096);
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
        if(strcmp(s->name, ".debug_abbrev") == 0)
        {
            debugAbbrev = (u8*)be_malloc(s->fileSize);
            debugAbbrevSize = s->fileSize;
            elf.readSection(s, m_debugAbbrev);
        }
        if(strcmp(s->name, ".debug_info") == 0)
        {
            debugInfo = (u8*)be_malloc(s->fileSize);
            debugInfoSize = s->fileSize;
            elf.readSection(s, m_debugInfo);
        }
        if(strcmp(s->name, ".debug_line") == 0)
        {
            lineProgram = (u8*)be_malloc(s->fileSize);
            elf.readSection(s, m_lineProgram);
        }
    }

    Dwarf::Parser<endianness> parser(debugAbbrev, debugAbbrevSize);
    while (parser)
    {
        CompilationUnit* unit = new CompilationUnit;
        parser >> *unit;
        if(!m_root)
        {
            m_root = unit;
        }
        else
        {
            m_root->insert(unit);
        }
    }

    be_free(lineProgram);
    be_free(debugInfo);
}

}}
