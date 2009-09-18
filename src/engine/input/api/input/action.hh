/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

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


    be_metaclass(INPUTEXPORT,Action,Object)
    be_properties
    be_end
};

}}

/*****************************************************************************/
#endif
