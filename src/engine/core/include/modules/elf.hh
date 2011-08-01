/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_RUNTIME_ELF_HH_
#define BE_CORE_RUNTIME_ELF_HH_
/*****************************************************************************/
#include    <core/runtime/module.hh>

namespace BugEngine { namespace Runtime
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



class Elf : public Module
{
    BE_NOCOPY(Elf);
private:
    template< ElfClass klass, ElfEndianness endianness >
    void parse(FILE* file);
private:
    ElfClass                m_class;
    ElfEndianness           m_endianness;
public:
    Elf(const char* filename, u64 baseAddress);
    ~Elf();

    operator void*() const { return (void*)(m_endianness != msb_invalid); }
    bool operator !() const { return m_endianness == msb_invalid; }
private:
    virtual Endianness endianness() const override { return m_endianness == msb_littleendian ? Endianness_Little : Endianness_Big; };
    virtual SymbolResolver::SymbolInformations getSymbolInformation() const override;
};

}}

/*****************************************************************************/
#endif

