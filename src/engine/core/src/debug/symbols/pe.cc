/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>

#include    <pe.hh>

namespace BugEngine { namespace Debug
{

#define be_enum(name)   \
    struct name { enum _##name; }; enum name::_##name
static inline char be_hex2char(u8 value)
{
    return (value&0xf) > 9 ? (value-10+'A') : (value+'0');
}

struct Guid
{
    u32 data1;
    u16 data2;
    u16 data3;
    u8  data4[8];

    struct GuidString
    {
        char str[40];
        operator char*()                { return str; }
        operator const char*() const    { return str; }
    };
    GuidString compactstr() const
    {
        GuidString result;
        result[0] = be_hex2char((data1 >> 28) & 0xf);
        result[1] = be_hex2char((data1 >> 24) & 0xf);
        result[2] = be_hex2char((data1 >> 20) & 0xf);
        result[3] = be_hex2char((data1 >> 16) & 0xf);
        result[4] = be_hex2char((data1 >> 12) & 0xf);
        result[5] = be_hex2char((data1 >> 8) & 0xf);
        result[6] = be_hex2char((data1 >> 4) & 0xf);
        result[7] = be_hex2char((data1 >> 0) & 0xf);

        result[8] = be_hex2char((data2 >> 12) & 0xf);
        result[9] = be_hex2char((data2 >> 8) & 0xf);
        result[10] = be_hex2char((data2 >> 4) & 0xf);
        result[11] = be_hex2char((data2 >> 0) & 0xf);

        result[12] = be_hex2char((data3 >> 12) & 0xf);
        result[13] = be_hex2char((data3 >> 8) & 0xf);
        result[14] = be_hex2char((data3 >> 4) & 0xf);
        result[15] = be_hex2char((data3 >> 0) & 0xf);

        result[16] = be_hex2char((data4[0] >> 4) & 0xf);
        result[17] = be_hex2char((data4[0]) & 0xf);
        result[18] = be_hex2char((data4[1] >> 4) & 0xf);
        result[19] = be_hex2char((data4[1]) & 0xf);
        result[20] = be_hex2char((data4[2] >> 4) & 0xf);
        result[21] = be_hex2char((data4[2]) & 0xf);
        result[22] = be_hex2char((data4[3] >> 4) & 0xf);
        result[23] = be_hex2char((data4[3]) & 0xf);
        result[24] = be_hex2char((data4[4] >> 4) & 0xf);
        result[25] = be_hex2char((data4[4]) & 0xf);
        result[26] = be_hex2char((data4[5] >> 4) & 0xf);
        result[27] = be_hex2char((data4[5]) & 0xf);
        result[28] = be_hex2char((data4[6] >> 4) & 0xf);
        result[29] = be_hex2char((data4[6]) & 0xf);
        result[30] = be_hex2char((data4[7] >> 4) & 0xf);
        result[31] = be_hex2char((data4[7]) & 0xf);
        result[32] = 0;
        return result;
    }
    GuidString str() const
    {
        GuidString result;
        result[0] = '{';
        result[1] = be_hex2char((data1 >> 28) & 0xf);
        result[2] = be_hex2char((data1 >> 24) & 0xf);
        result[3] = be_hex2char((data1 >> 20) & 0xf);
        result[4] = be_hex2char((data1 >> 16) & 0xf);
        result[5] = be_hex2char((data1 >> 12) & 0xf);
        result[6] = be_hex2char((data1 >> 8) & 0xf);
        result[7] = be_hex2char((data1 >> 4) & 0xf);
        result[8] = be_hex2char((data1 >> 0) & 0xf);
        result[9] = '-';
        result[10] = be_hex2char((data2 >> 12) & 0xf);
        result[11] = be_hex2char((data2 >> 8) & 0xf);
        result[12] = be_hex2char((data2 >> 4) & 0xf);
        result[13] = be_hex2char((data2 >> 0) & 0xf);
        result[14] = '-';
        result[15] = be_hex2char((data3 >> 12) & 0xf);
        result[16] = be_hex2char((data3 >> 8) & 0xf);
        result[17] = be_hex2char((data3 >> 4) & 0xf);
        result[18] = be_hex2char((data3 >> 0) & 0xf);
        result[19] = '-';
        result[20] = be_hex2char((data4[0] >> 4) & 0xf);
        result[21] = be_hex2char((data4[0]) & 0xf);
        result[22] = be_hex2char((data4[1] >> 4) & 0xf);
        result[23] = be_hex2char((data4[1]) & 0xf);
        result[24] = be_hex2char((data4[2] >> 4) & 0xf);
        result[25] = be_hex2char((data4[2]) & 0xf);
        result[26] = be_hex2char((data4[3] >> 4) & 0xf);
        result[27] = be_hex2char((data4[3]) & 0xf);
        result[28] = be_hex2char((data4[4] >> 4) & 0xf);
        result[29] = be_hex2char((data4[4]) & 0xf);
        result[30] = be_hex2char((data4[5] >> 4) & 0xf);
        result[31] = be_hex2char((data4[5]) & 0xf);
        result[32] = be_hex2char((data4[6] >> 4) & 0xf);
        result[33] = be_hex2char((data4[6]) & 0xf);
        result[34] = be_hex2char((data4[7] >> 4) & 0xf);
        result[35] = be_hex2char((data4[7]) & 0xf);
        result[36] = '}';
        result[37] = 0;
        return result;
    }
};

struct MSDosHeader
{
    u8  signature[2];
    u8  pad[0x3c-2];
    u32 offset;
};

struct ImageHeader
{
    be_enum(Machine)
    {
        Unknown = 0x0,
        AM33 = 0x1d3,
        AMD64 = 0x8664,
        ARM = 0x1c0,
        EBC = 0xebc,
        i386 = 0x14c,
        IA64 = 0x200,
        M32R = 0x9041,
        MIPS16 = 0x266,
        MIPSFPU = 0x366,
        MIPSFPU16 = 0x466,
        POWERPC = 0x1f0,
        POWERPCFP = 0x1f1,
        R4000 = 0x166,
        SH3 = 0x1a2,
        SH3DSP = 0x1a3,
        SH4 = 0x1a6,
        SH5 = 0x1a8,
        THUMB = 0x1c2,
        WCEMIPSV2 = 0x169
    };
    be_enum(Flags)
    {
        RelocsStripped = 0x1,
        ExecutableImage = 0x2,
        LinesStripped = 0x4,
        SymbolsStripped = 0x8,
        AggressiveWorkingSetTrim = 0x10,
        LargeAddressAware = 0x20,
        Reserved = 0x40,
        LittleEndian = 0x80,
        Machine32 = 0x100,
        DebugStripped = 0x200,
        RemovableRunFromSwap = 0x400,
        NetRunFromSwap = 0x800,
        SystemFile = 0x1000,
        Dll = 0x2000,
        UniProcessor = 0x4000,
        BigEndian = 0x8000
    };
    be_enum(Headers)
    {
        Pe32Header = 0x10b,
        Pe32PlusHeader = 0x20b
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
    be_enum(Subsystems)
    {
        Unknown = 0,
        Native = 1,
        Windows = 2,
        Console = 3,
        Posix = 7,
        CE = 9,
        EFI = 10,
        EFIBoot = 11,
        EFIRuntime = 12,
        EFIRom = 13,
        XBox = 14,
    };
    be_enum(DataDirectories)
    {
        ExportTable = 0,
        ImportTable = 1,
        ResourceTable = 2,
        ExceptionTable = 3,
        CertificateTable = 4,
        BaseRelocationTable = 5,
        Debug = 6,
        Architecture = 7,
        GlobalPtr = 8,
        TLSTable = 9,
        LoadConfigTable = 10,
        BoundImport = 11,
        ImportAddressTable = 12,
        DelayImportDescriptor = 13,
        CLRRuntimeHeeader = 14,
        Reserved = 15,
        Count = 16
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
    u32 size;
    u32 offset;
    u32 rawDataSize;
    u32 rawDataOffset;
    u32 relocationOffset;
    u32 lineNumbersOffset;
    u16 relocationCount;
    u16 lineNumbersCount;
    u32 characteristics;
};


struct DebugEntry
{
    be_enum(Type)
    {
        Unknown = 0,
        COFF = 1,
        CodeView = 2,
        FramePointerOmission = 3,
        Misc = 4,
        Exception = 5,
        Fixup = 6,
        Mappings = 7,
        ReverseMappings = 8,
        Borland = 9,
        Reserved = 10,
        ClsId = 11
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
    char    signature[4];
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

PE::PE(const char *filename, FILE* f)
{
    be_info("loading file %s" | filename);
    MSDosHeader dosh;
    ImageHeader   imageHeader;
    fread(&dosh, sizeof(dosh), 1, f);
    fseek(f, dosh.offset, SEEK_SET);
    fread(&imageHeader, sizeof(imageHeader), 1, f);
    size_t debugEntryVirtualAdress = 0;
    size_t debugEntrySize = 0;
    {
        Malloc::MemoryBlock<u8> block(imageHeader.optionalHeaderSize);
        fread(block.data, imageHeader.optionalHeaderSize, 1, f);
        if(*(i16*)block.data == ImageHeader::Headers::Pe32Header)
        {
            PEHeader* header = reinterpret_cast<PEHeader*>(block.data);
            if(header->windows.dataDirectoryCount > PEHeader::DataDirectories::Debug)
            {
                debugEntryVirtualAdress = header->windows.dataDirectoryEntries[PEHeader::DataDirectories::Debug].offset;
                debugEntrySize = header->windows.dataDirectoryEntries[PEHeader::DataDirectories::Debug].size;
            }
        }
        else if(*(i16*)block.data == ImageHeader::Headers::Pe32PlusHeader)
        {
            PEPlusHeader* header = reinterpret_cast<PEPlusHeader*>(block.data);
            if(header->windows.dataDirectoryCount > PEHeader::DataDirectories::Debug)
            {
                debugEntryVirtualAdress = header->windows.dataDirectoryEntries[PEHeader::DataDirectories::Debug].offset;
                debugEntrySize = header->windows.dataDirectoryEntries[PEHeader::DataDirectories::Debug].size;
            }
        }
        else
        {
            be_notreached();
        }
    }
    if(debugEntryVirtualAdress && debugEntrySize)
    {
        be_assert(debugEntrySize % sizeof(DebugEntry) == 0, "got an unexpected size for the debug section; expected a multiple of %d, fot %d" | sizeof(DebugEntry) | debugEntrySize);
        size_t debugEntryCount = debugEntrySize / sizeof(DebugEntry);
        Malloc::MemoryBlock<SectionHeader> sections(imageHeader.sectionCount);
        fread(sections.data, sizeof(SectionHeader), imageHeader.sectionCount, f);
        for(u16 section = 0; section <  imageHeader.sectionCount; ++section)
        {
            if(sections[section].offset <= debugEntryVirtualAdress && (sections[section].offset + sections[section].size) > debugEntryVirtualAdress)
            {
                be_info("loading debug info from section %s" | sections[section].name);
                Malloc::MemoryBlock<DebugEntry> entries(debugEntryCount);
                fseek(f, sections[section].rawDataOffset + (debugEntryVirtualAdress - sections[section].offset), SEEK_SET);
                fread(entries, sizeof(DebugEntry), debugEntryCount, f);
                for(size_t i = 0; i < debugEntryCount; ++i)
                {
                    switch(entries[i].type)
                    {
                    case DebugEntry::Type::CodeView:
                        {
                            Malloc::MemoryBlock<u8> info(entries[i].size);
                            fseek(f, entries[i].fileOffset, SEEK_SET);
                            fread(info, entries[i].size, 1, f);
                            PdbInfo* pdbInfo = reinterpret_cast<PdbInfo*>(info.data);
                            if (strncmp(pdbInfo->signature, "NB10", 4) == 0)
                            {
                                PdbInfo20* pdb20 = reinterpret_cast<PdbInfo20*>(info.data);
                                be_info("loading debug info from file %s" | pdb20->filename);
                            }
                            else if (strncmp(pdbInfo->signature, "RSDS", 4) == 0)
                            {
                                PdbInfo70* pdb70 = reinterpret_cast<PdbInfo70*>(info.data);
                                be_info("loading debug info PDB7 from file %s" | pdb70->filename);
                                minitl::format<> debugID = minitl::format<>("%s%d") | pdb70->signature.compactstr() | pdb70->age;
                                be_info("GUID: %s, path:%s/%s/%s" | pdb70->signature.str() | pdb70->filename | debugID | pdb70->filename);
                            }
                        }
                        break;
                    case DebugEntry::Type::Reserved:
                        /* lots of Microsoft DLLs seem to have this debug entry... */
                        break;
                    default:
                        be_info("unsupported debug file format : %d" | entries[i].type);
                    }
                }
            }
        }
    }
    else
    {
        be_info("No debug information available");
    }
}

PE::~PE()
{
}

}}

