/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_INPUT_CONTROL_HH_
#define BE_INPUT_CONTROL_HH_
/*****************************************************************************/

namespace BugEngine { namespace Input
{

class be_api(INPUT) Control : public Object
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

    be_metaclass(INPUT,Control,Object)
    be_properties
    be_end
};

}}

/*****************************************************************************/
#endif
