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

#include    <system/stdafx.h>
#include    <system/sequencer.hh>

namespace BugEngine
{

const double Sequencer::s_maxFrameTime = 1.0/30.0;

METACLASS_IMPL("",Sequencer);

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

void Sequencer::addSequencer(refptr<Sequencer> seq, bool adjustLifeTime)
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
            refptr<Action> action = *it;
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
    Assert(m_pauseCount != 0);
}

void Sequencer::resume()
{
    Assert(m_pauseCount != 0);
    m_pauseCount--;
}

void Sequencer::run(refptr<Action> action)
{
    m_actions.insert(action);
}

void Sequencer::stop(refptr<Action> action)
{
    m_actions.erase(action);
}

}
