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

#ifndef BE_INPUT_GROUPCONTROL_HH_
#define BE_INPUT_GROUPCONTROL_HH_
/*****************************************************************************/
#include    <input/control.hh>

namespace BugEngine { namespace Input
{

class GroupControl : public Control
{
private:
    std::vector< refptr<Control> >  m_controls;
public:
    GroupControl(const istring& name);
    ~GroupControl();

    float           value() const override;
    bool            isDown() const override;
    bool            wasDown() const override;
    bool            wentDown() const override;
    bool            wentUp() const override;

    void add(Control* ctrl);

    METACLASS(INPUTEXPORT,GroupControl,Control)
    PROPERTIES
    END
};

}}

/*****************************************************************************/
#endif
