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
#include    <core/memory/malloc.hh>
#include    <core/debug/memory/debugger.hh>
#include    <core/debug/memory/internal.hh>

#include    <core/debugrenderer/debugrenderer.hh>

#include    <malloc.h>
#include    <core/debug/callstack.hh>

#include    <malloc.h>


namespace BugEngine
{

static void init()
{
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF);
#ifdef BE_ENABLE_MEMORY_TRACKING
    static Memory::Debugger::Scope scope(new Memory::InternalDebugger);
#endif
}


void* Malloc::systemAlloc(size_t size, size_t alignment)
{
#ifdef _MSC_VER
    return _aligned_malloc(size, alignment);
#else
    return malloc(size);
#endif
}

void* Malloc::systemRealloc(void* ptr, size_t size, size_t alignment)
{
#ifdef _MSC_VER
    return _aligned_realloc(ptr, size, alignment);
#else
    return realloc(ptr, size);
#endif
}

void Malloc::systemFree(void* pointer)
{
#ifdef _MSC_VER
    return _aligned_free(pointer);
#else
    return free(pointer);
#endif
}

BE_NOINLINE void* Malloc::internalAlloc(size_t size, size_t alignment, size_t skipStack)
{
    static bool initialized;
    if(!initialized)
    {
        initialized = true;
        init();
    }
    void* ptr = systemAlloc(size, alignment);
#ifdef BE_ENABLE_MEMORY_TRACKING
    Memory::Debugger::onAllocation(ptr, size, skipStack+1);
#endif
    return ptr;
}

BE_NOINLINE void* Malloc::internalRealloc(void* ptr, size_t size, size_t alignment, size_t skipStack)
{
#ifdef BE_ENABLE_MEMORY_TRACKING
    if(ptr)
    {
        Memory::Debugger::onRelease(ptr, skipStack+1);
    }
#endif
    ptr = systemRealloc(ptr, size, alignment);
#ifdef BE_ENABLE_MEMORY_TRACKING
    if(ptr)
    {
        Memory::Debugger::onAllocation(ptr, size, skipStack+1);
    }
#endif
    return ptr;
}

void Malloc::internalFree(void* ptr, size_t skipStack)
{
    if(! ptr)
        return;
#ifdef BE_ENABLE_MEMORY_TRACKING
    Memory::Debugger::onRelease(ptr, skipStack+1);
#endif
    systemFree(ptr);
}

void Malloc::frameUpdate()
{
#ifdef BE_ENABLE_MEMORY_TRACKING
    Memory::Debugger::onFrameUpdate();
#endif
}

}
