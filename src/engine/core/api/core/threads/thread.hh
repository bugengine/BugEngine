/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_CORE_THREADS_THREAD_HH_
#define BE_CORE_THREADS_THREAD_HH_
/*****************************************************************************/

namespace BugEngine
{

class be_api(CORE) Thread
{
private:
    class ThreadParams;
private:
    ThreadParams*   m_params;
    void*           m_data;
    unsigned long   m_id;
public:
    typedef intptr_t(*ThreadFunction)(intptr_t, intptr_t);
public:
    enum Priority
    {
        Idle,
        Lowest,
        BelowNormal,
        Normal,
        AboveNormal,
        Highest,
        Critical
    };
public:
    Thread(const istring& name, ThreadFunction f, intptr_t p1 = 0, intptr_t p2 = 0, Priority p = Normal, bool isSuspended = false);
    ~Thread();

    void resume();

    void setPriority(Priority p);

    unsigned long   id() const;
    void wait() const;

    static void             sleep(int milliseconds);
    static void             yield();
    static unsigned long    currentId();
    static const istring&   name();
};

}

/*****************************************************************************/
#endif
