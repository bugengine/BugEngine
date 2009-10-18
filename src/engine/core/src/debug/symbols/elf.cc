/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>

#include    <elf.hh>
#include    <dwarf.hh>

namespace BugEngine { namespace Debug
{

struct ElfIdentification
{
    u8  header[4];
    u8  klass;
    u8  msb;
    u8  version;
    u8  padding[9];
};

template< ElfClass, ElfEndianness >
struct ElfTypes;

template< >
struct ElfTypes<klass_32, msb_littleendian>
{
    typedef u16_l   half_t;
    typedef u32_l   word_t;
    typedef i32_l   sword_t;
    typedef u64_l   xword_t;
    typedef i64_l   sxword_t;
    typedef u32_l   addr_t;
    typedef u32_l   offset_t;
    typedef u16_l   section_t;
    typedef half_t  versym_t;
};

template< >
struct ElfTypes<klass_64, msb_littleendian>
{
    typedef u16_l   half_t;
    typedef u32_l   word_t;
    typedef i32_l   sword_t;
    typedef u64_l   xword_t;
    typedef i64_l   sxword_t;
    typedef u64_l   addr_t;
    typedef u64_l   offset_t;
    typedef u16_l   section_t;
    typedef half_t  versym_t;

};

template< >
struct ElfTypes<klass_32, msb_bigendian>
{
    typedef u16_b   half_t;
    typedef u32_b   word_t;
    typedef i32_b   sword_t;
    typedef u64_b   xword_t;
    typedef i64_b   sxword_t;
    typedef u32_b   addr_t;
    typedef u32_b   offset_t;
    typedef u16_b   section_t;
    typedef half_t  versym_t;
};

template< >
struct ElfTypes<klass_64, msb_bigendian>
{
    typedef u16_b   half_t;
    typedef u32_b   word_t;
    typedef i32_b   sword_t;
    typedef u64_b   xword_t;
    typedef i64_b   sxword_t;
    typedef u64_b   addr_t;
    typedef u64_b   offset_t;
    typedef u16_b   section_t;
    typedef half_t  versym_t;
};

template< ElfClass klass, ElfEndianness endianness >
struct ElfHeader
{
    typename ElfTypes<klass, endianness>::half_t     type;
    typename ElfTypes<klass, endianness>::half_t     machine;
    typename ElfTypes<klass, endianness>::word_t     version;
    typename ElfTypes<klass, endianness>::addr_t     entry;
    typename ElfTypes<klass, endianness>::offset_t   phoffset;
    typename ElfTypes<klass, endianness>::offset_t   shoffset;
    typename ElfTypes<klass, endianness>::word_t     flags;
    typename ElfTypes<klass, endianness>::half_t     ehsize;
    typename ElfTypes<klass, endianness>::half_t     phentsize;
    typename ElfTypes<klass, endianness>::half_t     phnum;
    typename ElfTypes<klass, endianness>::half_t     shentsize;
    typename ElfTypes<klass, endianness>::half_t     shnum;
    typename ElfTypes<klass, endianness>::half_t     shstrndx;
};

template< ElfClass klass, ElfEndianness endianness >
struct ElfSectionHeader
{
    enum Types
    {
        Null = 0,
        ProgBits = 1,
        SymbolTable = 2,
        StringTable = 3,
        Rela = 4,
        Hash = 5,
        Dynamic = 6,
        Note = 7,
        Nobits = 8,
        Rel = 9,
        SharedLibrary = 10,
        DynSym = 11
    };
    enum Flags
    {
        Write = 0x1,
        Alloc = 0x2,
        Exec = 0x4
    };
    typename ElfTypes<klass, endianness>::word_t     name;
    typename ElfTypes<klass, endianness>::word_t     type;
    typename ElfTypes<klass, endianness>::addr_t     flags;
    typename ElfTypes<klass, endianness>::addr_t     addr;
    typename ElfTypes<klass, endianness>::offset_t   offset;
    typename ElfTypes<klass, endianness>::addr_t     size;
    typename ElfTypes<klass, endianness>::word_t     link;
    typename ElfTypes<klass, endianness>::word_t     info;
    typename ElfTypes<klass, endianness>::addr_t     addralign;
    typename ElfTypes<klass, endianness>::addr_t     entsize;
};


static const char *s_elfFileType [] =
{
    "No file type",
    "Relocatable file",
    "Executable file",
    "Shared object file",
    "Core file",
    "Number of defined types"
};

static const char* s_elfMachineType [] =
{
"No machine",                                           //  0
"AT&T WE 32100",                                        //  1
"SUN SPARC",                                            //  2
"Intel 80386",                                          //  3
"Motorola m68k family",                                 //  4
"Motorola m88k family",                                 //  5
"Unknown machine",                                      //  6
"Intel 80860",                                          //  7
"MIPS R3000 big-endian",                                //  8
"IBM System/370",                                       //  9
"MIPS R3000 little-endian",                             // 10
"Unknown machine",                                      // 11
"Unknown machine",                                      // 12
"Unknown machine",                                      // 13
"Unknown machine",                                      // 14
"HPPA",                                                 // 15
"Unknown machine",                                      // 16
"Fujitsu VPP500",                                       // 17
"Sun's \"v8plus\"",                                     // 18
"Intel 80960",                                          // 19
"PowerPC",                                              // 20
"PowerPC 64-bit",                                       // 21
"IBM S390",                                             // 22
"Unknown machine",                                      // 23
"Unknown machine",                                      // 24
"Unknown machine",                                      // 25
"Unknown machine",                                      // 26
"Unknown machine",                                      // 27
"Unknown machine",                                      // 28
"Unknown machine",                                      // 29
"Unknown machine",                                      // 30
"Unknown machine",                                      // 31
"Unknown machine",                                      // 32
"Unknown machine",                                      // 33
"Unknown machine",                                      // 34
"Unknown machine",                                      // 35
"NEC V800 series",                                      // 36
"Fujitsu FR20",                                         // 37
"TRW RH-32",                                            // 38
"Motorola RCE",                                         // 39
"ARM",                                                  // 40
"Digital Alpha",                                        // 41
"Hitachi SH",                                           // 42
"SPARC v9 64-bit",                                      // 43
"Siemens Tricore",                                      // 44
"Argonaut RISC Core",                                   // 45
"Hitachi H8/300",                                       // 46
"Hitachi H8/300H",                                      // 47
"Hitachi H8S",                                          // 48
"Hitachi H8/500",                                       // 49
"Intel Merced",                                         // 50
"Stanford MIPS-X",                                      // 51
"Motorola Coldfire",                                    // 52
"Motorola M68HC12",                                     // 53
"Fujitsu MMA Multimedia Accelerator",                   // 54
"Siemens PCP",                                          // 55
"Sony nCPU embeeded RISC",                              // 56
"Denso NDR1 microprocessor",                            // 57
"Motorola Start*Core processor",                        // 58
"Toyota ME16 processor",                                // 59
"STMicroelectronic ST100 processor",                    // 60
"Advanced Logic Corp. Tinyj emb.fam",                   // 61
"AMD x86-64 architecture",                              // 62
"Sony DSP Processor",                                   // 63
"Unknown machine",                                      // 64
"Unknown machine",                                      // 65
"Siemens FX66 microcontroller",                         // 66
"STMicroelectronics ST9+ 8/16 mc",                      // 67
"STmicroelectronics ST7 8 bit mc",                      // 68
"Motorola MC68HC16 microcontroller",                    // 69
"Motorola MC68HC11 microcontroller",                    // 70
"Motorola MC68HC08 microcontroller",                    // 71
"Motorola MC68HC05 microcontroller",                    // 72
"Silicon Graphics SVx",                                 // 73
"STMicroelectronics ST19 8 bit mc",                     // 74
"Digital VAX",                                          // 75
"Axis Communications 32-bit embedded processor",        // 76
"Infineon Technologies 32-bit embedded processor",      // 77
"Element 14 64-bit DSP Processor",                      // 78
"LSI Logic 16-bit DSP Processor",                       // 79
"Donald Knuth's educational 64-bit processor",          // 80
"Harvard University machine-independent object files",  // 81
"SiTera Prism",                                         // 82
"Atmel AVR 8-bit microcontroller",                      // 83
"Fujitsu FR30",                                         // 84
"Mitsubishi D10V",                                      // 85
"Mitsubishi D30V",                                      // 86
"NEC v850",                                             // 87
"Mitsubishi M32R",                                      // 88
"Matsushita MN10300",                                   // 89
"Matsushita MN10200",                                   // 90
"picoJava",                                             // 91
"OpenRISC 32-bit embedded processor",                   // 92
"ARC Cores Tangent-A5",                                 // 93
"Tensilica Xtensa Architecture",                        // 94
};

Elf::Elf(const ifilename& filename)
    :   m_filename(filename)
    ,   m_stringPool(0)
    ,   m_class(klass_invalid)
    ,   m_endianness(msb_invalid)
    ,   m_file(fopen(filename.str().c_str(), "rb"))
{
    be_debug("loading file %s" | filename);
    ElfIdentification id;
    fread(&id, 1, sizeof(id), m_file);
    be_assert(id.header[0] == 0x7f && id.header[1] == 'E' && id.header[2] == 'L' && id.header[3] == 'F', "not a valid elf signature in file %s" | filename);
    m_class = (ElfClass)id.klass;
    m_endianness = (ElfEndianness)id.msb;
    if(id.klass == klass_32 && id.msb == msb_littleendian)
    {
        be_debug("32 bits little-endian");
        Elf::parse<klass_32, msb_littleendian>();
    }
    else if(id.klass == klass_64 && id.msb == msb_littleendian)
    {
        be_debug("64 bits little-endian");
        Elf::parse<klass_64, msb_littleendian>();
    }
    else if(id.klass == klass_32 && id.msb == msb_bigendian)
    {
        be_debug("32 bits big-endian");
        Elf::parse<klass_32, msb_bigendian>();
    }
    else if(id.klass == klass_64 && id.msb == msb_bigendian)
    {
        be_debug("64 bits big-endian");
        Elf::parse<klass_64, msb_bigendian>();
    }
    else
    {
        be_notreached();
    }
}

Elf::~Elf()
{
    be_free(const_cast<char*>(m_stringPool));
    fclose(m_file);
}

template< ElfClass klass, ElfEndianness e >
void Elf::parse()
{
    FILE* f = m_file;

    ElfHeader<klass, e> header;
    fread(&header, sizeof(header), 1, f);
    be_debug("elf file type: %s, for machine : %s" | s_elfFileType[header.type] | s_elfMachineType[header.machine]);
    UNUSED(BugEngine::Debug::s_elfFileType);
    UNUSED(BugEngine::Debug::s_elfMachineType);

    be_assert(header.shentsize == sizeof(ElfSectionHeader<klass, e>), "invalid or unsupported entry size; expected %d, got %d" | sizeof(ElfSectionHeader<klass, e>) | header.shentsize);
    ElfSectionHeader<klass, e> *sections = (ElfSectionHeader<klass, e>*)malloca(header.shentsize*header.shnum);
    fseek(f, checked_numcast<long>(header.shoffset), SEEK_SET);
    fread(sections, header.shentsize, header.shnum, f);

    {
        char* strings = (char*)be_malloc(checked_numcast<size_t>(sections[header.shstrndx].size));
        fseek(f, checked_numcast<long>(sections[header.shstrndx].offset), SEEK_SET);
        fread(strings, 1, checked_numcast<size_t>(sections[header.shstrndx].size), f);
        m_stringPool = strings;
    }
    
    for(int i = 0; i < header.shnum; ++i)
    {
        Section sec = { m_stringPool + sections[i].name, sections[i].addr, sections[i].size, sections[i].offset,  sections[i].size };
        m_sections.push_back(sec);
    }
    freea(sections);
}

refptr<const Symbols::ISymbolResolver> Elf::getSymbolResolver()
{
    for(const Section* s = begin(); s != end(); ++s)
    {
        if(strncmp(s->name, ".text", 5) == 0)
        {
            return new DwarfModule(m_filename, *this, s->offset, s->size);
        }
    }
    return 0;
}

const Elf::Section* Elf::begin() const
{
    return &m_sections[0];
}

const Elf::Section* Elf::end() const
{
    return &m_sections[m_sections.size()];
}

void Elf::readSection(const Section* s, void* buffer) const
{
    fseek(m_file, s->fileOffset, SEEK_SET);
    fread(buffer, s->fileSize, 1, m_file);
}

}}

