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

#ifndef BE_INPUT_DEVICE_HH_
#define BE_INPUT_DEVICE_HH_
/*****************************************************************************/
#include    <input/control.hh>
#include    <rtti/namespace.hh>

namespace BugEngine { namespace Input
{

class INPUTEXPORT Device : public Object
{
private:
    refptr<RTTI::Namespace>         m_deviceNamespace;
    istring                         m_name;
    size_t                          m_numControls;
    float*                          m_active;
    float*                          m_lastFrame;
    float*                          m_buffer;
public:
    Device(const istring& name, size_t numKeys);
    ~Device();

    const float* getActiveBuffer() const;
    const float* getLastBuffer() const;
    float* getBuffer();

    void addControl(Control* ctrl);
    void addControlAlias(const istring& ns, const istring& alias, Control* ctrl);

    void update();

    const istring& name() const                 { return m_name; }
    const RTTI::Namespace* getNamespace() const { return m_deviceNamespace.get(); }

    be_metaclass(INPUTEXPORT,Device,Object)
    be_properties
    be_end
};

}}

/*****************************************************************************/
#endif
