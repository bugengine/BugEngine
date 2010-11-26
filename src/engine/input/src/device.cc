/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <input/stdafx.h>
#include    <input/device.hh>
#include    <minitl/container/algorithm.hh>

namespace BugEngine { namespace Input
{


Device::Device(const istring& name, size_t numControls)
:   /*m_deviceNamespace(RTTI::Namespace::root()->createNamespace("input")->createNamespace("devices")->createNamespace(name))
,*/   m_name(name)
,   m_numControls(numControls)
,   m_active(numControls)
,   m_lastFrame(numControls)
,   m_buffer(numControls)
{
    minitl::fill((float*)m_active, (float*)m_active+numControls, 0.0f);
    minitl::fill((float*)m_lastFrame, (float*)m_lastFrame+numControls, 0.0f);
    minitl::fill((float*)m_buffer, (float*)m_buffer+numControls, 0.0f);
}

Device::~Device()
{
    //m_deviceNamespace->clear();
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

void Device::addControl(ref<Control> /*ctrl*/)
{
    //m_deviceNamespace->set(ctrl->name(), Value(ref<Object>(ctrl)));
}

void Device::addControlAlias(const istring& /*ns*/, const istring& /*alias*/, ref<Control> /*ctrl*/)
{
    //m_deviceNamespace->createNamespace(ns)->set(alias, Value(ref<Object>(ctrl)));
}

void Device::update()
{
    m_buffer.swap(m_lastFrame);
    m_lastFrame.swap(m_active);
    memcpy((float*)m_buffer, (float*)m_active, m_numControls*sizeof(float));
}

}}
