/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_SYSTEM_UPDATABLE_HH_
#define BE_SYSTEM_UPDATABLE_HH_
/*****************************************************************************/

namespace BugEngine
{

class Sequencer;

class SYSTEMEXPORT ActionContext
{
public:
    class StackFrame;
    friend class Sequencer;
    friend class StackFrame;
private:
    std::vector< Value >    m_stack;
    double                  m_deltaTime;
    size_t                  m_currentFrame;
    size_t                  m_frameSize;
    StackFrame*             m_frame;
public:
    class StackFrame
    {
        friend class ActionContext;
    private:
        ActionContext*  m_context;
        StackFrame*     m_previous;
        size_t          m_start;
        size_t          m_size;

        const size_t    m_local;
        const size_t    m_caller;
        size_t          m_callee;
    public:
        StackFrame(ActionContext* context, size_t nbargs, size_t locals);
        ~StackFrame();

        Value& operator[](int index);
        const Value& operator[](int index) const;
    
        void   push(const Value& value);
        void   pop();
        void   popall();
        size_t args() const;
    private:
        StackFrame(const StackFrame& other);
        StackFrame& operator=(const StackFrame& other);
    };

    ActionContext();
    ~ActionContext();

    double deltaTime() const { return m_deltaTime; }

    StackFrame* thisframe() { return m_frame; }
};

class SYSTEMEXPORT Action : public Object
{
public:
    Action();
    ~Action();

    virtual void initialize(ActionContext* context) const = 0;
    virtual bool oneturn(ActionContext* context) const = 0;

    be_metaclass(SYSTEMEXPORT,Action,Object)
    be_properties
    be_end
};

}

/*****************************************************************************/
#endif
