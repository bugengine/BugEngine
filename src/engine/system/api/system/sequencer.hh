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

#ifndef BE_SYSTEM_UPDATER_HH_
#define BE_SYSTEM_UPDATER_HH_
/*****************************************************************************/
#include    <system/action.hh>
#include    <core/timer.hh>

#include    <rtti/value.hh>

namespace BugEngine
{

class SYSTEMEXPORT Sequencer : public Object
{
public:
    typedef std::set< refptr<Action> >                          ActionList;
    typedef std::vector< std::pair<refptr<Sequencer>,bool> >    SequencerList;
private:
    SequencerList   m_children;
    ActionContext   m_context;
    Timer           m_frameTimer;
    ActionList      m_actions;
    size_t          m_pauseCount;
    bool            m_mandatory;
private:
    static const double s_maxFrameTime;
protected:
    virtual bool _oneturn(ActionContext* ctx);
public:
    Sequencer();
    ~Sequencer();

    void addSequencer(refptr<Sequencer> seq, bool adjustLifeTime);

    void init();

    void run(refptr<Action> action);
    void stop(refptr<Action> action);

    bool oneturn();
    void pause();
    void resume();

    be_metaclass(SYSTEMEXPORT,Sequencer,Object)
    be_properties
        be_method(init);
        be_method(addSequencer);
        be_method(oneturn);
        be_method(pause);
        be_method(resume);

        be_method(run);
        be_method(stop);
    be_end
};

}

/*****************************************************************************/
#endif
