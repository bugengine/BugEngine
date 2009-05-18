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

#ifndef BE_INPUT_ACTION_HH_
#define BE_INPUT_ACTION_HH_
/*****************************************************************************/

namespace BugEngine { namespace Input
{

class Control;
class InputMap;

class INPUTEXPORT Action : public Object
{
    friend class InputMap;
private:
    istring const           m_name;
    const InputMap* const   m_owner;
    std::vector< Control* > m_controls;
private:
    Action(const istring& name, const InputMap* owner);
public:
    ~Action();

public:
    const istring&  name() const;

    void            mapControl(Control* c);
    void            unmapControl(Control* c);

    float           value() const;
    bool            isDown() const;
    bool            wentDown() const;
    bool            wentUp() const;


    METACLASS(INPUTEXPORT,Action,Object)
    PROPERTIES
    END
};

}}

/*****************************************************************************/
#endif
