/* BugEngine <bugengine.devel@gmail.com>
   see LICENSE for detail */

#include <bugengine/plugin.debug.runtime/stdafx.h>

#include <pe.hh>

namespace BugEngine { namespace Runtime {

static inline char be_hex2char(u8 value)
{
    return (value & (char)0xf) > 9 ? (char)(value - 10 + 'A') : (char)(value + '0');
}

struct Guid
{
    u32 data1;
    u16 data2;
    u16 data3;
    u8  data4[8];

    struct GuidString
    {
        char str[33];
             operator char*()
        {
            return str;
        }
        operator const char*() const
        {
            return str;
        }
    };
    GuidString compactstr() const
    {
        GuidString result;
        result[0] = be_hex2char(u8((data1 >> 28) & 0xf));
        result[1] = be_hex2char(u8((data1 >> 24) & 0xf));
        result[2] = be_hex2char(u8((data1 >> 20) & 0xf));
        result[3] = be_hex2char(u8((data1 >> 16) & 0xf));
        result[4] = be_hex2char(u8((data1 >> 12) & 0xf));
        result[5] = be_hex2char(u8((data1 >> 8) & 0xf));
        result[6] = be_hex2char(u8((data1 >> 4) & 0xf));
        result[7] = be_hex2char(u8((data1 >> 0) & 0xf));

        result[8]  = be_hex2char(u8((data2 >> 12) & 0xf));
        result[9]  = be_hex2char(u8((data2 >> 8) & 0xf));
        result[10] = be_hex2char(u8((data2 >> 4) & 0xf));
        result[11] = be_hex2char(u8((data2 >> 0) & 0xf));

        result[12] = be_hex2char(u8((data3 >> 12) & 0xf));
        result[13] = be_hex2char(u8((data3 >> 8) & 0xf));
        result[14] = be_hex2char(u8((data3 >> 4) & 0xf));
        result[15] = be_hex2char(u8((data3 >> 0) & 0xf));

        result[16] = be_hex2char(u8((data4[0] >> 4) & 0xf));
        result[17] = be_hex2char(u8((data4[0]) & 0xf));
        result[18] = be_hex2char(u8((data4[1] >> 4) & 0xf));
        result[19] = be_hex2char(u8((data4[1]) & 0xf));
        result[20] = be_hex2char(u8((data4[2] >> 4) & 0xf));
        result[21] = be_hex2char(u8((data4[2]) & 0xf));
        result[22] = be_hex2char(u8((data4[3] >> 4) & 0xf));
        result[23] = be_hex2char(u8((data4[3]) & 0xf));
        result[24] = be_hex2char(u8((data4[4] >> 4) & 0xf));
        result[25] = be_hex2char(u8((data4[4]) & 0xf));
        result[26] = be_hex2char(u8((data4[5] >> 4) & 0xf));
        result[27] = be_hex2char(u8((data4[5]) & 0xf));
        result[28] = be_hex2char(u8((data4[6] >> 4) & 0xf));
        result[29] = be_hex2char(u8((data4[6]) & 0xf));
        result[30] = be_hex2char(u8((data4[7] >> 4) & 0xf));
        result[31] = be_hex2char(u8((data4[7]) & 0xf));
        result[32] = 0;
        return result;
    }
    GuidString str() const
    {
        GuidString result;
        result[0]  = '{';
        result[1]  = be_hex2char(u8((data1 >> 28) & 0xf));
        result[2]  = be_hex2char(u8((data1 >> 24) & 0xf));
        result[3]  = be_hex2char(u8((data1 >> 20) & 0xf));
        result[4]  = be_hex2char(u8((data1 >> 16) & 0xf));
        result[5]  = be_hex2char(u8((data1 >> 12) & 0xf));
        result[6]  = be_hex2char(u8((data1 >> 8) & 0xf));
        result[7]  = be_hex2char(u8((data1 >> 4) & 0xf));
        result[8]  = be_hex2char(u8((data1 >> 0) & 0xf));
        result[9]  = '-';
        result[10] = be_hex2char(u8((data2 >> 12) & 0xf));
        result[11] = be_hex2char(u8((data2 >> 8) & 0xf));
        result[12] = be_hex2char(u8((data2 >> 4) & 0xf));
        result[13] = be_hex2char(u8((data2 >> 0) & 0xf));
        result[14] = '-';
        result[15] = be_hex2char(u8((data3 >> 12) & 0xf));
        result[16] = be_hex2char(u8((data3 >> 8) & 0xf));
        result[17] = be_hex2char(u8((data3 >> 4) & 0xf));
        result[18] = be_hex2char(u8((data3 >> 0) & 0xf));
        result[19] = '-';
        result[20] = be_hex2char(u8((data4[0] >> 4) & 0xf));
        result[21] = be_hex2char(u8((data4[0]) & 0xf));
        result[22] = be_hex2char(u8((data4[1] >> 4) & 0xf));
        result[23] = be_hex2char(u8((data4[1]) & 0xf));
        result[24] = be_hex2char(u8((data4[2] >> 4) & 0xf));
        result[25] = be_hex2char(u8((data4[2]) & 0xf));
        result[26] = be_hex2char(u8((data4[3] >> 4) & 0xf));
        result[27] = be_hex2char(u8((data4[3]) & 0xf));
        result[28] = be_hex2char(u8((data4[4] >> 4) & 0xf));
        result[29] = be_hex2char(u8((data4[4]) & 0xf));
        result[30] = be_hex2char(u8((data4[5] >> 4) & 0xf));
        result[31] = be_hex2char(u8((data4[5]) & 0xf));
        result[32] = be_hex2char(u8((data4[6] >> 4) & 0xf));
        result[33] = be_hex2char(u8((data4[6]) & 0xf));
        result[34] = be_hex2char(u8((data4[7] >> 4) & 0xf));
        result[35] = be_hex2char(u8((data4[7]) & 0xf));
        result[36] = '}';
        result[37] = 0;
        return result;
    }
};

struct MSDosHeader
{
    u8  signature[2];
    u8  pad[0x3c - 2];
    u32 offset;
};

struct ImageHeader
{
    enum Machines
    {
        Machine_Unknown   = 0x0,
        Machine_AM33      = 0x1d3,
        Machine_AMD64     = 0x8664,
        Machine_ARM       = 0x1c0,
        Machine_EBC       = 0xebc,
        Machine_i386      = 0x14c,
        Machine_IA64      = 0x200,
        Machine_M32R      = 0x9041,
        Machine_MIPS16    = 0x266,
        Machine_MIPSFPU   = 0x366,
        Machine_MIPSFPU16 = 0x466,
        Machine_POWERPC   = 0x1f0,
        Machine_POWERPCFP = 0x1f1,
        Machine_R4000     = 0x166,
        Machine_SH3       = 0x1a2,
        Machine_SH3DSP    = 0x1a3,
        Machine_SH4       = 0x1a6,
        Machine_SH5       = 0x1a8,
        Machine_THUMB     = 0x1c2,
        Machine_WCEMIPSV2 = 0x169
    };
    enum Flags
    {
        Flag_RelocsStripped           = 0x1,
        Flag_ExecutableImage          = 0x2,
        Flag_LinesStripped            = 0x4,
        Flag_SymbolsStripped          = 0x8,
        Flag_AggressiveWorkingSetTrim = 0x10,
        Flag_LargeAddressAware        = 0x20,
        Flag_Reserved                 = 0x40,
        Flag_LittleEndian             = 0x80,
        Flag_Machine32                = 0x100,
        Flag_DebugStripped            = 0x200,
        Flag_RemovableRunFromSwap     = 0x400,
        Flag_NetRunFromSwap           = 0x800,
        Flag_SystemFile               = 0x1000,
        Flag_Dll                      = 0x2000,
        Flag_UniProcessor             = 0x4000,
        Flag_BigEndian                = 0x8000
    };
    enum Headers
    {
        Header_Pe32Header     = 0x10b,
        Header_Pe32PlusHeader = 0x20b
    };
    u8  signature[4];
    u16 machine;
    u16 sectionCount;
    u32 timestamp;
    u32 symbolTableOffset;
    u32 symbolCount;
    u16 optionalHeaderSize;
    u16 characteristics;
};

struct PEHeader
{
    enum Subsystems
    {
        Subsystem_Unknown    = 0,
        Subsystem_Native     = 1,
        Subsystem_Windows    = 2,
        Subsystem_Console    = 3,
        Subsystem_Posix      = 7,
        Subsystem_CE         = 9,
        Subsystem_EFI        = 10,
        Subsystem_EFIBoot    = 11,
        Subsystem_EFIRuntime = 12,
        Subsystem_EFIRom     = 13,
        Subsystem_XBox       = 14
    };
    enum DataDirectories
    {
        DataDirectory_ExportTable           = 0,
        DataDirectory_ImportTable           = 1,
        DataDirectory_ResourceTable         = 2,
        DataDirectory_ExceptionTable        = 3,
        DataDirectory_CertificateTable      = 4,
        DataDirectory_BaseRelocationTable   = 5,
        DataDirectory_Debug                 = 6,
        DataDirectory_Architecture          = 7,
        DataDirectory_GlobalPtr             = 8,
        DataDirectory_TLSTable              = 9,
        DataDirectory_LoadConfigTable       = 10,
        DataDirectory_BoundImport           = 11,
        DataDirectory_ImportAddressTable    = 12,
        DataDirectory_DelayImportDescriptor = 13,
        DataDirectory_CLRRuntimeHeeader     = 14,
        DataDirectory_Reserved              = 15,
        DataDirectory_Count                 = 16
    };
    struct
    {
        u16 type;
        u8  linkerMajor;
        u8  linkerMinor;
        u32 codeSize;
        u32 initializedDataSize;
        u32 uninitializedDataSize;
        u32 entryPointOffset;
        u32 codeBase;
        u32 dataBase;
    } standard;
    struct
    {
        u32 imageBase;
        u32 sectionAlignment;
        u32 fileAlignment;
        u16 majorOSVersion;
        u16 minorOSVersion;
        u16 majorImageVersion;
        u16 minorImageVersion;
        u16 majorSubsystemVersion;
        u16 minorSubsystemVersion;
        u32 win32Version;
        u32 imageSize;
        u32 headersSize;
        u32 checksum;
        u16 subsystem;
        u16 characteristics;
        u32 stackReserveSize;
        u32 stackCommitSize;
        u32 heapReserveSize;
        u32 heapCommitSize;
        u32 loaderFlags;
        u32 dataDirectoryCount;
        struct
        {
            u32 offset;
            u32 size;
        } dataDirectoryEntries[1];
    } windows;
};

struct PEPlusHeader
{
    struct
    {
        u16 type;
        u8  linkerMajor;
        u8  linkerMinor;
        u32 codeSize;
        u32 initializedDataSize;
        u32 uninitializedDataSize;
        u32 entryPointOffset;
        u32 codeBase;
    } standard;
    struct
    {
        u64 imageBase;
        u32 sectionAlignment;
        u32 fileAlignment;
        u16 majorOSVersion;
        u16 minorOSVersion;
        u16 majorImageVersion;
        u16 minorImageVersion;
        u16 majorSubsystemVersion;
        u16 minorSubsystemVersion;
        u32 win32Version;
        u32 imageSize;
        u32 headersSize;
        u32 checksum;
        u16 sunsystem;
        u16 characteristics;
        u64 stackReserveSize;
        u64 stackCommitSize;
        u64 heapReserveSize;
        u64 heapCommitSize;
        u32 loaderFlags;
        u32 dataDirectoryCount;
        struct
        {
            u32 offset;
            u32 size;
        } dataDirectoryEntries[1];
    } windows;
};

struct SectionHeader
{
    char name[8];
    u32  size;
    u32  offset;
    u32  rawDataSize;
    u32  rawDataOffset;
    u32  relocationOffset;
    u32  lineNumbersOffset;
    u16  relocationCount;
    u16  lineNumbersCount;
    u32  characteristics;
};

struct DebugEntry
{
    enum Types
    {
        Type_Unknown              = 0,
        Type_COFF                 = 1,
        Type_CodeView             = 2,
        Type_FramePointerOmission = 3,
        Type_Misc                 = 4,
        Type_Exception            = 5,
        Type_Fixup                = 6,
        Type_Mappings             = 7,
        Type_ReverseMappings      = 8,
        Type_Borland              = 9,
        Type_Reserved             = 10,
        Type_ClsId                = 11
    };
    u32 characteristics;
    u32 timeStamp;
    u16 major;
    u16 minor;
    u32 type;
    u32 size;
    u32 imageOffset;
    u32 fileOffset;
};

struct PdbInfo
{
    char signature[4];
};

struct PdbInfo20
{
    PdbInfo info;
    u32     offset;
    u32     timestamp;
    u32     age;
    char    filename[1];
};

struct PdbInfo70
{
    PdbInfo info;
    Guid    signature;
    u32     age;
    char    filename[1];
};

struct COFFSymbol
{
    char name[8];
    u32  value;
    u16  section;
    u16  type;
    u8   storage;
    u8   auxilliary;
};

PE::PE(const char* filename, u64 baseAddress) : Module(filename, baseAddress)
{
    be_info("loading file %s" | filename);
    /*MSDosHeader dosh;
    ImageHeader   imageHeader;
    m_file = fopen(filename, "rb");
    if (m_file)
    {
        fread(&dosh, sizeof(dosh), 1, m_file);
        fseek(m_file, dosh.offset, SEEK_SET);
        fread(&imageHeader, sizeof(imageHeader), 1, m_file);
        size_t debugEntryVirtualAdress = 0;
        size_t debugEntrySize = 0;
        {
            minitl::Allocator::Block<u8> block(Arena::stack(), imageHeader.optionalHeaderSize);
            fread(block, imageHeader.optionalHeaderSize, 1, m_file);
            if (*(i16*)(u8*)block == ImageHeader::Header_Pe32Header)
            {
                PEHeader* header = reinterpret_cast<PEHeader*>((u8*)block);
                if (header->windows.dataDirectoryCount > PEHeader::DataDirectory_Debug)
                {
                    debugEntryVirtualAdress =
    header->windows.dataDirectoryEntries[PEHeader::DataDirectory_Debug].offset; debugEntrySize =
    header->windows.dataDirectoryEntries[PEHeader::DataDirectory_Debug].size;
                }
            }
            else if (*(i16*)(u8*)block == ImageHeader::Header_Pe32PlusHeader)
            {
                PEPlusHeader* header = reinterpret_cast<PEPlusHeader*>((u8*)block);
                if (header->windows.dataDirectoryCount > PEHeader::DataDirectory_Debug)
                {
                    debugEntryVirtualAdress =
    header->windows.dataDirectoryEntries[PEHeader::DataDirectory_Debug].offset; debugEntrySize =
    header->windows.dataDirectoryEntries[PEHeader::DataDirectory_Debug].size;
                }
            }
            else
            {
                be_notreached();
            }
        }

        minitl::Allocator::Block<SectionHeader> sections(Arena::stack(), imageHeader.sectionCount);
        fread(sections, sizeof(SectionHeader), imageHeader.sectionCount, m_file);

        if (debugEntryVirtualAdress && debugEntrySize)
        {
            be_assert(debugEntrySize % sizeof(DebugEntry) == 0, "got an unexpected size for the
    debug section; expected a multiple of %d, got %d" | sizeof(DebugEntry) | debugEntrySize); size_t
    debugEntryCount = debugEntrySize / sizeof(DebugEntry); for (u16 section = 0; section <
    imageHeader.sectionCount; ++section)
            {
                if (sections[section].offset <= debugEntryVirtualAdress && (sections[section].offset
    + sections[section].size) > debugEntryVirtualAdress)
                {
                    be_info("loading debug info from section %s" | sections[section].name);
                    minitl::Allocator::Block<DebugEntry> entries(Arena::stack(), debugEntryCount);
                    fseek(m_file, static_cast<long>(sections[section].rawDataOffset +
    (debugEntryVirtualAdress - sections[section].offset)), SEEK_SET); fread(entries,
    sizeof(DebugEntry), debugEntryCount, m_file); for (size_t i = 0; i < debugEntryCount; ++i)
                    {
                        switch(entries[i].type)
                        {
                        case DebugEntry::Type_CodeView:
                            {
                                minitl::Allocator::Block<u8> info(Arena::stack(), entries[i].size);
                                fseek(m_file, entries[i].fileOffset, SEEK_SET);
                                fread(info, entries[i].size, 1, m_file);
                                PdbInfo* pdbInfo = reinterpret_cast<PdbInfo*>((u8*)info);
                                if (strncmp(pdbInfo->signature, "NB10", 4) == 0)
                                {
                                    PdbInfo20* pdb20 = reinterpret_cast<PdbInfo20*>((u8*)info);
                                    m_symbolInformations.type =
    SymbolResolver::SymbolInformations::PDB20; m_symbolInformations.filename = pdb20->filename;
                                    m_symbolInformations.identifier =
    istring(minitl::format<128u>("%d%d") | pdb20->timestamp | pdb20->age);
                                    m_symbolInformations.offset = m_baseAddress;
                                }
                                else if (strncmp(pdbInfo->signature, "RSDS", 4) == 0)
                                {
                                    PdbInfo70* pdb70 = reinterpret_cast<PdbInfo70*>((u8*)info);
                                    m_symbolInformations.type =
    SymbolResolver::SymbolInformations::PDB70; m_symbolInformations.filename = pdb70->filename;
                                    m_symbolInformations.identifier =
    istring(minitl::format<128u>("%s%d") | (const char *)pdb70->signature.compactstr() |
    pdb70->age); m_symbolInformations.offset = m_baseAddress;
                                }
                            }
                            break;
                        case DebugEntry::Type_Reserved:
                            // lots of Microsoft DLLs seem to have this debug entry...
                            // I don't really care about it though
                            break;
                        default:
                            be_info("unsupported debug file format : %d" | entries[i].type);
                        }
                    }
                }
            }
        }

        u32 stringTableSize;
        size_t stringTableOffset = imageHeader.symbolTableOffset + imageHeader.symbolCount*18;
        fseek(m_file, static_cast<long>(stringTableOffset), SEEK_SET);
        fread(&stringTableSize, sizeof(stringTableSize), 1, m_file);
        minitl::Allocator::Block<char> stringBlock(Arena::stack(), stringTableSize);
        StringTable* strings = reinterpret_cast<StringTable*>((char*)stringBlock);
        strings->size = stringTableSize;
        fread(strings->strings, 1, stringTableSize-4, m_file);

        for (u16 section = 0; section <  imageHeader.sectionCount; ++section)
        {
            char* name = sections[section].name;
            if (name[0] == '/')
            {
                int offset = 0;
                for (int i = 1; i < 8 && name[i]; ++i)
                {
                    be_assert(name[i] >= '0' && name[i] <= '9', "unexpected character in section
    name %s : %c" | name | name[i]); offset = offset*10 + name[i]-'0';
                }
                name = strings->strings+offset-4;
            }
            Section sec = { name, sections[section].offset,  sections[section].size,
    sections[section].rawDataOffset, sections[section].rawDataSize }; m_sections.push_back(sec);
        }
        const Section& code = (*this)[".text"];
        be_assert(code, "No .text section in executable %s" | m_filename);
        const Section& debug_link = (*this)[".gnu_debuglink"];
        if (debug_link)
        {
            minitl::Allocator::Block<char> f(Arena::stack(),
    be_checked_numcast<size_t>(debug_link.fileSize)); readSection(debug_link, f);
            m_symbolInformations.type = SymbolResolver::SymbolInformations::PEDwarf;
            m_symbolInformations.filename = ifilename(f);
            m_symbolInformations.offset = m_baseAddress + code.offset;
            m_symbolInformations.size = code.size;
        }
        else if ((*this)[".debug_info"])
        {
            m_symbolInformations.type = SymbolResolver::SymbolInformations::PEDwarf;
            m_symbolInformations.filename = m_filename;
            m_symbolInformations.offset = m_baseAddress + code.offset;
            m_symbolInformations.size = code.size;
        }
    }*/
}

PE::~PE()
{
}

SymbolResolver::SymbolInformations PE::getSymbolInformation() const
{
    return m_symbolInformations;
}

}}  // namespace BugEngine::Runtime
