/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <python/stdafx.h>
#include    <python/context.hh>

namespace BugEngine { namespace Python
{

Context::Context(const Plugin::Context& context, ref<PythonLibrary> library)
    :   ScriptEngine<PythonScript>(Arena::python(), context.resourceManager)
    ,   m_library(library)
    ,   m_pythonState(library->createThread())
{
}

Context::~Context()
{
    m_library->destroyThread(m_pythonState);
}

void Context::unload(Resource::Resource& /*handle*/)
{
    PythonLibrary::ThreadLock lock(m_library, m_pythonState);
}

void Context::runBuffer(weak<const PythonScript> /*script*/, Resource::Resource& /*resource*/,
                        const minitl::Allocator::Block<u8>& block)
{
    PythonLibrary::ThreadLock lock(m_library, m_pythonState);
    (*m_library->m_PyRun_SimpleString)((const char*)block.begin());
}

void Context::reloadBuffer(weak<const PythonScript> /*script*/, Resource::Resource& /*resource*/,
                           const minitl::Allocator::Block<u8>& block)
{
    PythonLibrary::ThreadLock lock(m_library, m_pythonState);
    (*m_library->m_PyRun_SimpleString)((const char*)block.begin());
}

}}
