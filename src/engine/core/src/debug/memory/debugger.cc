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
#include    <core/debug/memory/debugger.hh>
#include    <core/debug/memory/internal.hh>
#include    <core/debug/memory/network.hh>

#ifdef BE_ENABLE_MEMORY_TRACKING

namespace BugEngine { namespace Memory
{

static const size_t g_maxDebuggerCount = 4;

class NullDebugger : public Debugger
{
private:
    virtual void registerAllocation(void* pointer, size_t size, int threadid, Callstack::Address* from, size_t adressSize) override {};
    virtual void registerDeallocation(void* pointer, int threadid, Callstack::Address* from, size_t adressSize) override {};
    virtual void frameUpdate() override {};
public:
    NullDebugger();
    ~NullDebugger();

    static NullDebugger* instance();
};

class Debugger::RefPointer
{
private:
    minitl::iptr<Debugger>  m_debugger;
public:
    RefPointer(Debugger* d = NullDebugger::instance());
    RefPointer(RefPointer& other);
    ~RefPointer();

    bool set(Debugger* d);
    void clear();
    Debugger* operator->();
    bool operator==(const Debugger*) const;
};




Debugger::RefPointer* Debugger::getDebuggers()
{
    static Debugger::RefPointer g_debuggers[g_maxDebuggerCount];
    return g_debuggers;
}

void Debugger::onAllocation(void *pointer, size_t size, size_t skipStack)
{
    static const size_t depth = 128;
    Callstack::Address callstack[depth];
    Callstack::backtrace(callstack, 128, skipStack);
    RefPointer* debuggers = getDebuggers();
    for(size_t i = 0; i < g_maxDebuggerCount; ++i)
    {
        RefPointer r = debuggers[i];
        r->registerAllocation(pointer, size, Thread::currentId(), callstack, depth);
    }
}

void Debugger::onRelease(void *pointer, size_t skipStack)
{
    static const size_t depth = 128;
    Callstack::Address callstack[depth];
    Callstack::backtrace(callstack, 128, skipStack);
    RefPointer* debuggers = getDebuggers();
    for(size_t i = 0; i < g_maxDebuggerCount; ++i)
    {
        RefPointer r = debuggers[i];
        r->registerDeallocation(pointer, Thread::currentId(), callstack, depth);
    }
}

void Debugger::onFrameUpdate()
{
    RefPointer* debuggers = getDebuggers();
    for(size_t i = 0; i < g_maxDebuggerCount; ++i)
    {
        RefPointer r = debuggers[i];
        r->frameUpdate();
    }
}

Debugger::Debugger()
:   m_refCount(0)
{
}

Debugger::~Debugger()
{
    Assert(m_refCount == 0);
}

void Debugger::connect()
{
    RefPointer p(this);
    RefPointer* debuggers = getDebuggers();
    for(size_t i = 0; i < g_maxDebuggerCount; ++i)
    {
        if(debuggers[i].set(this))
            return;
    }
    Assert(false);
}

void Debugger::disconnect()
{
    RefPointer* debuggers = getDebuggers();
    for(size_t i = 0; i < g_maxDebuggerCount; ++i)
    {
        if(debuggers[i] == this)
            debuggers[i].clear();
    }
}

Debugger::RefPointer::RefPointer(Debugger* d)
:   m_debugger(d)
{
    m_debugger->m_refCount++;
}

Debugger::RefPointer::RefPointer(RefPointer& other)
:   m_debugger(other.m_debugger)
{
    other.m_debugger->m_refCount++;
}

Debugger::RefPointer::~RefPointer()
{
    if(!--m_debugger->m_refCount)
        delete (Debugger*)m_debugger;
}

bool Debugger::RefPointer::set(Debugger* d)
{
    if(m_debugger.setConditional(d, NullDebugger::instance()) == NullDebugger::instance())
    {
        d->m_refCount++;
        return true;
    }
    return false;
}

void Debugger::RefPointer::clear()
{
    Assert(m_debugger != NullDebugger::instance());
    Debugger* d = m_debugger;
    m_debugger = NullDebugger::instance();
    if(!--d->m_refCount)
        delete d;
}

Debugger* Debugger::RefPointer::operator->()
{
    return m_debugger;
}

bool Debugger::RefPointer::operator==(const Debugger* debugger) const
{
    return m_debugger == debugger;
}

NullDebugger::NullDebugger()
:   Debugger()
{
    m_refCount = 1;
}

NullDebugger::~NullDebugger()
{
    Assert(m_refCount == 1);
    m_refCount = 0;
}

NullDebugger* NullDebugger::instance()
{
    static NullDebugger g_instance;
    return &g_instance;
}

}}

#endif

