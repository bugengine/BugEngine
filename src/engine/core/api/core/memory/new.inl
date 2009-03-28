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

#ifndef BE_CORE_MEMORY_NEW_INL_
#define BE_CORE_MEMORY_NEW_INL_
/*****************************************************************************/

void* operator new(size_t size)
{
    return BugEngine::Malloc::internalAlloc(size, 16, 2);
}

void* operator new(size_t size, size_t alignment)
{
    return BugEngine::Malloc::internalAlloc(size, alignment, 2);
}

void* operator new[](size_t size)
{
    return BugEngine::Malloc::internalAlloc(size, 16, 2);
}

void operator delete(void* ptr)
{
    return BugEngine::Malloc::internalFree(ptr, 2);
}

void operator delete[](void* ptr)
{
    return BugEngine::Malloc::internalFree(ptr, 2);
}

/*****************************************************************************/
#endif

