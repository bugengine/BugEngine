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

#ifndef BE_INPUT_INPUTMAP_HH_
#define BE_INPUT_INPUTMAP_HH_
/*****************************************************************************/
#include    <input/control.hh>
#include    <input/action.hh>

namespace BugEngine { namespace Input
{

class Manager;

class INPUTEXPORT InputMap : public Object
{
private:
    std::vector< refptr<Action> >   m_actions;
    bool                            m_active;
public:
    typedef size_t  ActionId;
public:
    InputMap();
    ~InputMap();

    Action* addAction(ActionId id, const istring& name);

    void    mapControl(ActionId id, Control* button);
    bool    isDown(ActionId id) const;
    bool    wentDown(ActionId id) const;
    bool    wentUp(ActionId id) const;
    float   value(ActionId id) const;

    bool    active() const;
    void    setActive(bool active);

    be_metaclass(INPUTEXPORT,InputMap,Object)
    private:
        scopedptr<Manager>  m_manager;
        static void init(MetaClass* mc);
    public:
        const Manager* getManager() const;
    be_properties
        be_method(mapAction);
        be_method(isDown);
        be_method(wentDown);
        be_method(axisValue);
    be_end
};

}}

/*****************************************************************************/
#endif
