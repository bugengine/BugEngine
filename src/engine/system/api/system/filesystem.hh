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
    
#ifndef BE_SYSTEM_FILESYSTEM_HH_
#define BE_SYSTEM_FILESYSTEM_HH_
/*****************************************************************************/
#include    <core/utils/singleton.hh>
#include    <core/string/istring.hh>
#include    <core/log/exception.hh>

namespace BugEngine
{
   
class FileSystemComponent;
class AbstractMemoryStream;



enum FileOpenMode
{
    eReadOnly,
    eReadWrite
};

//lint -esym(1712,EFileNotFound)
class EFileNotFound : public EException
{
public:
    EFileNotFound(const ifilename& file) :
        EException("file " + file.str() + " does not exist")
    {
    }
};

//lint -esym(1712,ENoCreate)
class ENoCreate : public EException
{
public:
    ENoCreate(const ifilename& file) :
        EException("cannot create file " + file.str())
    {
    }
};

//lint -esym(1712,EReadOnly)
class EReadOnly : public EException
{
public:
    EReadOnly(const ifilename& file) :
        EException("cannot open file " + file.str() + " in read-write mode")
    {
    }
};

class SYSTEMEXPORT FileSystem : public Singleton<FileSystem>
{
private:
    class FileSystemMountPoint;
private:
    FileSystemMountPoint*   m_root;
public:
    FileSystem(void);
    ~FileSystem(void);

    void mount(const ipath& prefix, refptr<const FileSystemComponent> component);
    void umount(const ipath& prefix);
    refptr<AbstractMemoryStream> open(const ifilename& file, FileOpenMode mode) const;
    size_t age(const ifilename& file) const;

    std::set<ifilename> listFiles(const ipath& prefix, const char* extension = 0);
    std::set<ipath> listDirectories(const ipath& prefix);
};

}


/*****************************************************************************/
#endif
