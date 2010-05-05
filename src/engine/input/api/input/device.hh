/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_INPUT_DEVICE_HH_
#define BE_INPUT_DEVICE_HH_
/*****************************************************************************/
#include    <input/control.hh>

namespace BugEngine { namespace Input
{

class be_api(INPUT) Device : public minitl::refcountable
{
private:
    //ref<RTTI::Namespace>         m_deviceNamespace;
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

    void addControl(ref<Control> ctrl);
    void addControlAlias(const istring& ns, const istring& alias, ref<Control> ctrl);

    void update();

    const istring& name() const                 { return m_name; }
    //const weak<RTTI::Namespace> getNamespace() const { return m_deviceNamespace; }
};

}}

/*****************************************************************************/
#endif
