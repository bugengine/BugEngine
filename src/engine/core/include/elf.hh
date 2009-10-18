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
public:
    struct Section
    {
        const char *    name;
        u64             offset;
        u64             size;
        u64             fileOffset;
        u64             fileSize;
    };
private:
    template< ElfClass klass, ElfEndianness endianness >
    void parse();
private:
    const ifilename         m_filename;
    std::vector<Section>    m_sections;
    const char *            m_stringPool;
    ElfClass                m_class;
    ElfEndianness           m_endianness;
    FILE*                   m_file;
public:
    Elf(const ifilename& filename);
    ~Elf();

    refptr<const Symbols::ISymbolResolver> getSymbolResolver();

    const Section* begin() const;
    const Section* end() const;

    void readSection(const Section* section, void* buffer) const;

    ElfEndianness endianness() const { return m_endianness; }
private:
    Elf(const Elf& other);
    Elf& operator=(const Elf& other);
};

}}

/*****************************************************************************/
#endif

