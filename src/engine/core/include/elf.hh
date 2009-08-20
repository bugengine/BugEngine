/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

#ifndef BE_CORE_DEBUG_ELF_HH_
#define BE_CORE_DEBUG_ELF_HH_
/*****************************************************************************/

namespace BugEngine { namespace Debug
{

enum ElfClass
{
    klass_invalid = 0,
    klass_32 = 1,
    klass_64 = 2
};

enum ElfEndianness
{
    msb_invalid = 0,
    msb_littleendian = 1,
    msb_bigendian = 2
};



class Elf
{
private:
    template< ElfClass klass, ElfEndianness endianness >
    void parse(FILE* f);
public:
    Elf(const char* filename, FILE* f);
    ~Elf();

};

}}

/*****************************************************************************/
#endif

