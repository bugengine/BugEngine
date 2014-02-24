/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#ifndef BE_FILESYSTEM_FILE_FILE_SCRIPT_HH_
#define BE_FILESYSTEM_FILE_FILE_SCRIPT_HH_
/**************************************************************************************************/
#include    <core/string/istring.hh>
#include    <kernel/interlocked_stack.hh>
#include    <minitl/intrusive_list.hh>

namespace BugEngine
{

class Folder;

class be_api(FILESYSTEM) File : public minitl::refcountable
{
    friend class Folder;
public:
    class Ticket;
    friend class Ticket;
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
protected:
    const ifilename m_filename;
    const Media     m_media;
    u64             m_size;
    u64             m_state;
protected:
    File(ifilename filename, Media media, u64 size, u64 state);
public:
    ~File();
public:
    class Ticket :  public minitl::refcountable
    {
        friend class File;
    public:
        enum Action
        {
            Read,
            Write
        };
        Action const                    action;
        weak<const File>                file;
        minitl::Allocator::Block<u8>    buffer;
        i_u32                           processed;
        const i64                       offset;
        const u32                       total;
        i_bool                          error;

        inline bool done() const    { return error || processed == total; }
    public:
        Ticket(minitl::Allocator& arena, weak<const File> file, i64 offset, u32 size);
        Ticket(minitl::Allocator& arena, weak<const File> file, i64 offset, u32 size, const void* data);
        ~Ticket();
    private:
        Ticket(const Ticket&);
        Ticket& operator=(const Ticket&);
    };

    ref<const Ticket> beginRead(u32 size = 0, i64 offset = 0, minitl::Allocator& arena = Arena::temporary()) const;
    ref<const Ticket> beginWrite(const void* data, u32 size, i64 offset = -1);

    void fillBuffer(weak<Ticket> ticket) const;
    void writeBuffer(weak<Ticket> ticket) const;

    u64 getState() const;
    bool isDeleted() const;
    bool isReloadable() const;

    ifilename getFileName() const { return m_filename; }
private:
    virtual void doFillBuffer(weak<Ticket> ticket) const = 0;
    virtual void doWriteBuffer(weak<Ticket> ticket) const = 0;
protected:
    void refresh(u64 fileSize, u64 state);
};

}

/**************************************************************************************************/
#endif
