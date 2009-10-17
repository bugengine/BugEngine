/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_DEBUG_ELF_HH_
#define BE_CORE_DEBUG_ELF_HH_
/*****************************************************************************/
#include    <core/debug/symbols.hh>

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
    void parse();
private:
    const char *                m_stringPool;
    FILE*                       m_file;
public:
    Elf(const ifilename& filename);
    ~Elf();

    refptr<const Symbols::ISymbolResolver> getSymbolResolver();
};

}}

/*****************************************************************************/
#endif

