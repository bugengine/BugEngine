/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_RUNTIME_MODULE_HH_
#define BE_CORE_RUNTIME_MODULE_HH_
/*****************************************************************************/
#include    <core/runtime/callstack.hh>
#include    <core/runtime/symbols.hh>

namespace BugEngine { namespace Runtime
{

class Symbol;
class SymbolResolver;

class Module : public minitl::refcountable<void>
{
public:
    class Section
    {
        friend class Module;
    public:
        istring         name;
        u64             offset;
        u64             size;
        u64             fileOffset;
        u64             fileSize;

        bool operator !() const { return offset == 0; }
        operator void* () const { return (void*)offset; }
    };
private:
    friend class Section;
protected:
    ifilename                   m_filename;
    u64                         m_baseAddress;
    refptr<const Module>        m_next;
    minitl::vector< Section >   m_sections;
public:
    Module(const char* filename, u64 baseAddress);
    virtual ~Module();

    virtual Endianness endianness() const = 0;

    const Section& operator[](const istring& index) const;

    void readSection(const Section& section, void* data) const;

    virtual SymbolResolver::SymbolInformations getSymbolInformation() const = 0;

    static refptr<const Module> self();
    refptr<const Module> next() const { return m_next; }
};

}}

/*****************************************************************************/
#endif
