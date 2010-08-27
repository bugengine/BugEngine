/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_INPUT_ACTION_HH_
#define BE_INPUT_ACTION_HH_
/*****************************************************************************/

namespace BugEngine { namespace Input
{

class Control;
class InputMap;

class be_api(INPUT) Action : public minitl::refcountable
{
private:
    istring const                               m_name;
    const InputMap* const                       m_owner;
    minitl::vector< Control*, Arena::General >  m_controls;
public:
    Action(const istring& name, const InputMap* owner);
    ~Action();

public:
    const istring&  name() const;

    void            mapControl(Control* c);
    void            unmapControl(Control* c);

    float           value() const;
    bool            isDown() const;
    bool            wentDown() const;
    bool            wentUp() const;
private:
    Action(const Action& other);
    Action& operator=(const Action& other);
};

}}

/*****************************************************************************/
#endif
