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

#ifndef BE_INPUT_CONTROL_HH_
#define BE_INPUT_CONTROL_HH_
/*****************************************************************************/

namespace BugEngine { namespace Input
{

class INPUTEXPORT Control : public Object
{
private:
    istring         m_name;
public:
    Control(const istring& name);
    ~Control();

            const istring&  name() const;
    virtual float           value() const = 0;
    virtual bool            isDown() const = 0;
    virtual bool            wasDown() const = 0;
    virtual bool            wentDown() const = 0;
    virtual bool            wentUp() const = 0;

    METACLASS(INPUTEXPORT,Control,Object)
    PROPERTIES
    END
};

}}

/*****************************************************************************/
#endif
