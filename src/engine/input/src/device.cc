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
