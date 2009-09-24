/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <core/stdafx.h>

#include    <pe.hh>

namespace BugEngine { namespace Debug
{

struct MSDosHeader
{
    u8  signature[2];
    u8  pad[0x3c-2];
    u32 offset;
};

struct ImageHeader
{
    enum
    {
        MachineUnknown = 0x0,
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
    enum
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
    enum
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
    enum
    {
        SubsystemUnknown = 0,
        SubsystemNative = 1,
        SubsystemWindows = 2,
        SubsystemConsole = 3,
        SubsystemPosix = 7,
        SubsystemCE = 9,
        SubsystemEFI = 10,
        SubsystemEFIBoot = 11,
        SubsystemEFIRuntime = 12,
        SubsystemEFIRom = 13,
        SubsystemXBox = 14,
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
    } Windows;
    struct
    {
        u32 offset;
        u32 size;
    } dataDirectoryEntries[1];
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
    } Windows;
    struct
    {
        u32 offset;
        u32 size;
    } dataDirectoryEntries[1];
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

PE::PE(const char *filename, FILE* f)
{
    be_info("loading file %s" | filename);
    MSDosHeader dosh;
    ImageHeader   imageHeader;
    fread(&dosh, sizeof(dosh), 1, f);
    fseek(f, dosh.offset, SEEK_SET);
    fread(&imageHeader, sizeof(imageHeader), 1, f);
    fseek(f, imageHeader.optionalHeaderSize, SEEK_CUR);
    for(u16 i = 0; i < imageHeader.sectionCount; ++i)
    {
        SectionHeader section;
        fread(&section, sizeof(section), 1, f);
    }
}

PE::~PE()
{
}

}}

