/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#ifndef BE_MAIN_APPLICATION_HH_
#define BE_MAIN_APPLICATION_HH_
/*****************************************************************************/
#include    <mobile/world.hh>
#include    <graphics/scene/iscene.hh>
#include    <graphics/renderer/rendertarget.hh>
#include    <graphics/renderer/renderbackend.hh>
#include    <system/scheduler/task/group.hh>

namespace BugEngine
{

class Application : public minitl::refcountable
{
private:
    struct Request;
private:
    class RenderView
    {
    private:
        ref<Graphics::IScene>       m_scene;
        ref<Graphics::RenderTarget> m_renderTarget;
        uint2                       m_viewport;
        ref<TaskGroup>              m_renderTask;
        std::vector< ref<ITask> >   m_tasks;
    public:
        RenderView(ref<Graphics::IScene> scene, ref<Graphics::RenderTarget> target);
        ~RenderView();

        weak<ITask> renderTask() const;
    };
private:
    scoped<Scheduler>                               m_scheduler;
    minitl::vector< RenderView >                    m_views;
    minitl::vector< ref<ITask> >                    m_tasks;
    minitl::istack<Request>                         m_requests;
private:
    void addSceneSync(ref<Graphics::IScene> scene, ref<Graphics::RenderTarget> target);
    void processRequests();
public:
    Application(int argc, const char *argv[]);
    virtual ~Application(void);

    int run(void);

    void addScene(ref<Graphics::IScene> scene, ref<Graphics::RenderTarget> target);

    weak<const Scheduler> scheduler() const  { return m_scheduler; }
private:
    Application(void);
    Application(const Application&);
    const Application& operator=(const Application&);
};

}

/*****************************************************************************/
#endif
