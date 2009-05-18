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
* Boston, MA                                                                  *
* 02110-1301  USA                                                             *
\*****************************************************************************/
    
#ifndef BE_SYSTEM_FILESYSTEM_DISK_HH_
#define BE_SYSTEM_FILESYSTEM_DISK_HH_
/*****************************************************************************/
#include    <system/fscomponent.hh>
#include    <core/string/istring.hh>


namespace BugEngine
{

class AbstractMemoryStream;


class SYSTEMEXPORT DiskFS : public FileSystemComponent
{
    friend class FileSystem;
private:
    ipath       m_prefix;
    bool        m_readOnly;
public:
    DiskFS(const ipath& prefix, bool readonly = false);
    ~DiskFS(void);

    virtual bool writable() const override;

    virtual refptr<AbstractMemoryStream> open(const ifilename& file, FileOpenMode mode) const override;
    virtual size_t age(const ifilename& file) const override;
};

}


/*****************************************************************************/
#endif
