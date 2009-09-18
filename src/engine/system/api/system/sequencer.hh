/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

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
