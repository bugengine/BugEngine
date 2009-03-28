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

        const size_t    m_caller;
        const size_t    m_local;
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

    METACLASS(SYSTEMEXPORT,Action,Object)
    PROPERTIES
    END
};

}

/*****************************************************************************/
#endif
