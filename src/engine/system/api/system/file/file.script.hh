/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_FILE_FILE_SCRIPT_HH_
#define BE_SYSTEM_FILE_FILE_SCRIPT_HH_
/*****************************************************************************/
#include    <core/string/istring.hh>

namespace BugEngine
{

class be_api(SYSTEM) File : public minitl::refcountable
{
public:
    struct Media
    {
        enum Type
        {
            Disk,
            Network,
            Dvd,
            Package,
            Unknown
        };
        Type    type;
        u32     index;
        u32     offset;
    };
private:
    const Media m_media;
    size_t      m_size;
protected:
    File(Media media, size_t size);
public:
    ~File();
public:
    class Ticket : public minitl::refcountable
    {
        friend class File;
    private:
        const weak<const File>      m_file;
    public:
        const Allocator::Block<u8>  buffer;
        i_size_t                    processed;
        const size_t                offset;
        const size_t                total;

        inline bool done() const    { return processed == total; }

    private:
        Ticket(Allocator& arena, weak<const File> file, size_t offset, size_t size);
        Ticket(const Ticket&);
        Ticket& operator=(const Ticket&);
    };

    ref<const Ticket> beginRead(size_t offset = 0, size_t size = 0, Allocator& arena = tempArena()) const;
    //const WriteTicket& beginWrite(Ticket& ticket, size_t offset = 0, size_t size = 0);
};

}

/*****************************************************************************/
#endif
