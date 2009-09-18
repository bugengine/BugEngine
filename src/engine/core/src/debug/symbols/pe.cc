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
    u8  signature[2];
    u8  linkerVersion;
    u8  linkerRevision;
    u16 entryTableOffset;
    u16 entryTableSize;
    u32 crc;
    u16 flags;
    u16 autoSegmentIndex;
    u16 autoSegmentHeapSize;
    u16 autoSegmentStackSize;
    u32 CSIP;
    u32 SSSP;
    u16 segmentCount;
    u16 moduleCount;
    u16 residentNameCount;
    u16 segmentTableOffset;
    u16 resourceTableOffset;
    u16 residentNameTableOffset;
    u16 moduleTableOffset;
    u16 importedNameTableOffset;
    u32 nonresidentNameTableOffset;
    u16 movableEntryCOunt;
    u16 segmentAlignmentShift;
    u16 resourceCount;
    u8  exeType;
};

enum ExeFlags
{
    SINGLEDATA = 0x0001,
    MULTIPLEDATA = 0x0002,
    INVALID = 0x2000,
    LIBRARY = 0x8000
};

enum ExeType
{
    WINDOWS = 0x02
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

