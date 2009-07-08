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

#include    <core/stdafx.h>
#include    <core/debug/memory/internal.hh>
#include    <core/debug/memory/location.hh>
#include    <core/debug/memory/tag.hh>

#ifdef BE_ENABLE_MEMORY_TRACKING

namespace BugEngine { namespace Memory
{

InternalDebugger::InternalDebugger()
:   Debugger()
{
}

InternalDebugger::~InternalDebugger()
{
}

void InternalDebugger::registerAllocation(void* pointer, size_t size, int threadid, Callstack::Address* from, size_t adressSize)
{
    UNUSED(threadid);
    UNUSED(adressSize);
    LocationInfo* l = LocationInfo::location(from[0]);
    MemoryTag::create(pointer, size, l);
}

void InternalDebugger::registerDeallocation(void* pointer, int threadid, Callstack::Address* from, size_t adressSize)
{
    UNUSED(threadid);
    UNUSED(adressSize);
    UNUSED(from);
    MemoryTag::MemoryTagInfo i;
    MemoryTag::release(pointer, i);
}

void InternalDebugger::frameUpdate()
{
}

}}

#endif
