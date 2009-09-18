/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

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

