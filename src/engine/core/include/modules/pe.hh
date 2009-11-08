/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_DEBUG_PE_HH_
#define BE_CORE_DEBUG_PE_HH_
/*****************************************************************************/
#include <core/runtime/symbols.hh>

namespace BugEngine { namespace Runtime
{

class PE
{
private:
    struct StringTable
    {
        u32     size;
        char    strings[1];
    };
    struct Section
    {
        const char *name;
        u64         offset;
        u64         size;
    };
private:
    FILE*                   m_file;
    const StringTable*      m_stringBuffer;
    std::vector<Section>    m_sections;
public:
    PE(const ifilename& filename);
    ~PE();

    refptr<const Symbols::ISymbolResolver> getSymbolResolver();
};

}}

/*****************************************************************************/
#endif

