/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

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
    be_end
};

}}

/*****************************************************************************/
#endif
