/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <system/stdafx.h>
#include    <system/sequencer.hh>

namespace BugEngine
{

const double Sequencer::s_maxFrameTime = 1.0/30.0;

be_metaclass_impl("",Sequencer);

Sequencer::Sequencer() :
    m_children(),
    m_context(),
    m_actions(),
    m_pauseCount(0)
{
}

Sequencer::~Sequencer()
{
}

void Sequencer::init()
{
    m_frameTimer.start();
    for(Sequencer::SequencerList::iterator it = m_children.begin(); it != m_children.end(); ++it)
    {
        it->first->init();
    }
}

void Sequencer::addSequencer(ref<Sequencer> seq, bool adjustLifeTime)
{
    m_children.push_back(std::make_pair(seq, adjustLifeTime));
}

bool Sequencer::_oneturn(ActionContext* ctx)
{
    bool end = false;
    for(Sequencer::SequencerList::iterator it = m_children.begin(); it != m_children.end(); ++it)
    {
        end |= it->first->oneturn() && it->second;
    }

    if(!end && m_pauseCount == 0)
    {
        ActionList::iterator it = m_actions.begin();
        while(it != m_actions.end())
        {
            ref<Action> action = *it;
            if(action->oneturn(ctx))
            {
                ActionList::iterator next = it;
                next++;
                m_actions.erase(it);
                it = next;
            }
            else
                ++it;
        }
    }

    return (m_children.empty() && m_actions.empty()) || end;
}

bool Sequencer::oneturn()
{
    m_context.m_deltaTime = m_frameTimer.tick();
    if(m_context.m_deltaTime > s_maxFrameTime)
        m_context.m_deltaTime = s_maxFrameTime;
    return _oneturn(&m_context);
}

void Sequencer::pause()
{
    m_pauseCount++;
}

void Sequencer::resume()
{
    be_assert(m_pauseCount != 0, "resuming sequencer that was not paused");
    m_pauseCount--;
}

void Sequencer::run(ref<Action> action)
{
    m_actions.insert(action);
}

void Sequencer::stop(ref<Action> action)
{
    m_actions.erase(action);
}

}
