/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_FILE_FILE_SCRIPT_HH_
#define BE_SYSTEM_FILE_FILE_SCRIPT_HH_
/*****************************************************************************/
#include    <core/string/istring.hh>

namespace BugEngine
{

struct IOContext;

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
        u64     offset;
        Media(Type t, u32 i, u64 o)
            :   type(t), index(i), offset(o)
        {
        }
    };
private:
    const Media m_media;
    u64         m_size;
protected:
    File(Media media, u64 size);
public:
    ~File();
public:
    class Ticket :  public minitl::refcountable
                 ,  public minitl::intrusive_list<Ticket>::item
    {
        friend struct IOContext;
    private:
        const weak<const File>      m_file;
    public:
        const Allocator::Block<u8>  buffer;
        i_u32                       processed;
        const u64                   offset;
        const u32                   total;

        inline bool done() const    { return processed == total; }

        Ticket(Allocator& arena, weak<const File> file, u64 offset, u32 size);
    private:
        Ticket(const Ticket&);
        Ticket& operator=(const Ticket&);
    };

    ref<const Ticket> beginRead(u64 offset = 0, u32 size = 0, Allocator& arena = tempArena()) const;
    //const WriteTicket& beginWrite(Ticket& ticket, size_t offset = 0, size_t size = 0);
};

}

/*****************************************************************************/
#endif
