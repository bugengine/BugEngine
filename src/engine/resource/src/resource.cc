/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <resource/stdafx.h>
#include    <resource/resource.hh>
#include    <rtti/engine/namespace.hh>

BE_REGISTER_NAMESPACE_2_(game, BugEngine, Resource);

namespace BugEngine { namespace Resource
{

static const Handle s_nullHandle = { 0, {0} };
Resource::Resource()
    :   m_handle(s_nullHandle)
{
}

Resource::~Resource()
{
    be_assert(m_handle.id.intId == 0, "resource handle destroyed but hasn't been properly unloaded");
}

Resource& Resource::null()
{
    static Resource s_nullResource;
    return s_nullResource;
}

void Resource::setRefHandle(ref<minitl::refcountable> handle)
{
    be_assert(m_handle.id.ptrId == 0, "setRefHandle called but handle already has a value; use clearRefHandle before calling setRefHandle");
    minitl::refcountable* object = handle.operator->();
    if (object)
    {
        object->addref();
        m_handle.id.ptrId = object;
    }
}

void Resource::clearRefHandle()
{
    minitl::refcountable* object = (minitl::refcountable*)m_handle.id.ptrId;
    if (object)
    {
        object->decref();
        m_handle.id.ptrId = 0;
    }
}

minitl::refcountable* Resource::getRefHandleInternal() const
{
    return (minitl::refcountable*)m_handle.id.ptrId;
}

}}
