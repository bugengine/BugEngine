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
    struct Section
    {
        const char *    name;
        u64             offset;
        u64             size;
    };
    std::vector<Section>    m_sections;
private:
    template< ElfClass klass, ElfEndianness endianness >
    void parse();
private:
    const ifilename m_filename;
    const char *    m_stringPool;
    FILE*           m_file;
public:
    Elf(const ifilename& filename);
    ~Elf();

    refptr<const Symbols::ISymbolResolver> getSymbolResolver();

    const Section* begin() const;
    const Section* end() const;
private:
    Elf(const Elf& other);
    Elf& operator=(const Elf& other);
};

}}

/*****************************************************************************/
#endif

