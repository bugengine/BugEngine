/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <input/stdafx.h>
#include    <input/device.hh>

#include    <rtti/namespace.hh>

namespace BugEngine { namespace Input
{

be_abstractmetaclass_impl("Input",Device);


Device::Device(const istring& name, size_t numControls)
:   m_deviceNamespace(RTTI::Namespace::root()->createNamespace("input")->createNamespace("devices")->createNamespace(name))
,   m_name(name)
,   m_numControls(numControls)
,   m_active((float*)be_malloc(sizeof(float)*numControls))
,   m_lastFrame((float*)be_malloc(sizeof(float)*numControls))
,   m_buffer((float*)be_malloc(sizeof(float)*numControls))
{
    std::fill(m_active, m_active+numControls, 0.0f);
    std::fill(m_lastFrame, m_lastFrame+numControls, 0.0f);
    std::fill(m_buffer, m_buffer+numControls, 0.0f);
}

Device::~Device()
{
    m_deviceNamespace->clear();
    be_free(m_buffer);
    be_free(m_lastFrame);
    be_free(m_active);
}

const float* Device::getActiveBuffer() const
{
    return m_active;
}

const float* Device::getLastBuffer() const
{
    return m_lastFrame;
}

float* Device::getBuffer()
{
    return m_buffer;
}

void Device::addControl(Control* ctrl)
{
    m_deviceNamespace->set(ctrl->name(), Value(refptr<Object>(ctrl)));
}

void Device::addControlAlias(const istring& ns, const istring& alias, Control* ctrl)
{
    m_deviceNamespace->createNamespace(ns)->set(alias, Value(refptr<Object>(ctrl)));
}

void Device::update()
{
    float* tmp = m_buffer;
    m_buffer = m_lastFrame;
    m_lastFrame = m_active;
    m_active = tmp;
    memcpy(m_buffer, m_active, m_numControls*sizeof(float));
}

}}
