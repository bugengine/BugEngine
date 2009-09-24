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

struct ExeHeader
{
    enum MachineType
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
    enum Characteristics
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
    u8  signature[4];
    u16 machine;
    u16 sectionCount;
    u32 timestamp;
    u32 symbolTableOffset;
    u32 symbolCount;
    u16 optionalHeaderSize;
    u16 characteristics;
};


PE::PE(const char *filename, FILE* f)
{
    be_info("loading file %s" | filename);
    MSDosHeader dosh;
    ExeHeader   exeh;
    fread(&dosh, sizeof(dosh), 1, f);
    fseek(f, dosh.offset, SEEK_SET);
    fread(&exeh, sizeof(exeh), 1, f);
}

PE::~PE()
{
}

}}

