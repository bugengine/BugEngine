/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_CORE_STREAMS_HH_
#define BE_CORE_STREAMS_HH_
/*****************************************************************************/

namespace BugEngine
{

class be_api(CORE) MemoryStream
{
private:
    minitl::Allocator::Block<u8>    m_memory;
    u64                             m_size;
    u64                             m_capacity;
public:
    MemoryStream(minitl::Allocator& allocator, u64 size = 0);
    ~MemoryStream();

    void*       memory()                                { return m_memory; }
    const void* memory() const                          { return m_memory; }
    u64         size() const                            { return m_size; }
    u64         capacity() const                        { return m_capacity; }
    void        resize(u64 size);
    void        write(const void *buffer, u64 size);
    void        erase(u64 count);
};

}

/*****************************************************************************/
#endif
