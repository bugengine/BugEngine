/*****************************************************************************\
* BugEngine                                                                   *
* Copyright (C) 2005-2008  screetch <screetch@gmail.com>                      *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
* License for more details.                                                   *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to                                   *
* the Free Software Foundation, Inc.,                                         *
* 51 Franklin St,                                                             *
* Fifth Floor,                                                                *
* Boston, MA 02110-1301                                                       *
* USA                                                                         *
\*****************************************************************************/

#ifndef BE_CORE_STREAMS_HH_
#define BE_CORE_STREAMS_HH_
/*****************************************************************************/
#include    <minitl/ptr/refcountable>

namespace BugEngine
{

class COREEXPORT AbstractMemoryStream : public minitl::refcountable<>
{
public:
    enum SeekMethod
    {
        eSeekFromStart = SEEK_SET,
        eSeekMove      = SEEK_CUR,
        eSeekFromEnd   = SEEK_END
    };
public:
    AbstractMemoryStream();
    virtual ~AbstractMemoryStream();

    virtual void*  basememory() = 0;
    virtual void*  memory();
    virtual i64    offset() const = 0;
    virtual i64    size() const = 0;
    virtual void   seek(SeekMethod method, i64 offset) = 0;
    virtual i64    read(void* buffer, i64 size);
    virtual void   write(void *buffer, i64 size);
    virtual void   resize(i64 size) = 0;
    virtual bool   writable() const = 0;
};
    
class COREEXPORT MemoryStream : public AbstractMemoryStream
{
private:
    void*   m_memory;
    i64     m_size;
    i64     m_offset;
public:
    MemoryStream();
    MemoryStream(i64 size);
    virtual ~MemoryStream();

    virtual void*  basememory() override;
    virtual i64    size() const override;
    virtual i64    offset() const override;
    virtual void   seek(SeekMethod method, i64 offset) override;
    virtual void   resize(i64 size) override;
    virtual bool   writable() const override;
};

}

/*****************************************************************************/
#endif
