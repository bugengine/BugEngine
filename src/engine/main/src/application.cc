/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <main/stdafx.h>
#include    <main/application.hh>

#include    <rtti/namespace.hh>
#include    <system/scheduler/task/group.hh>
#include    <system/scheduler/task/method.hh>

namespace BugEngine
{

be_abstractmetaclass_impl("",Application);

struct Application::Request : public minitl::inode
{
    enum
    {
        AddScene,
        RemoveScene
    } operation;
    ref<minitl::refcountable> param1;
    ref<minitl::refcountable> param2;
};


Application::RenderView::RenderView(ref<Graphics::Scene> scene, ref<Graphics::RenderTarget> target)
:   m_scene(scene)
,   m_renderTarget(target)
,   m_renderTask(ref<TaskGroup>::create("render", color32(255,0,0)))
{
    ref<ITask> renderTask = target->createSceneRenderTask(scene);

    m_renderTask->addStartTask(scene->updateTask());
    m_renderTask->addEndTask(renderTask);

    scene->updateTask()->addCallback(renderTask->startCallback());
    m_tasks.push_back(renderTask);
}

Application::RenderView::~RenderView()
{
}

weak<ITask> Application::RenderView::renderTask() const
{
    return m_renderTask;
}

Application::Application(int argc, const char *argv[])
:   Object()
,   m_scheduler(scoped<Scheduler>::create())
,   m_tasks()
{
    UNUSED(argc); UNUSED(argv);

    ref< TaskGroup > updateTask = ref< TaskGroup >::create("application", color32(255,255,0));
    m_tasks.push_back(updateTask);

    m_tasks.push_back(ref< Task< MethodCaller<Scheduler, &Scheduler::frameUpdate> > >::create("scheduler", color32(255,255,0), MethodCaller<Scheduler, &Scheduler::frameUpdate>(m_scheduler)));
    m_tasks.push_back(ref< Task< ProcedureCaller<&Malloc::frameUpdate> > >::create("memory", color32(255,255,0), ProcedureCaller<&Malloc::frameUpdate>()));
    m_tasks.push_back(ref< Task< MethodCaller<Application, &Application::processRequests> > >::create("requests", color32(255,255,0), MethodCaller<Application, &Application::processRequests>(this)));
    updateTask->addStartTask(m_tasks[1]);
    updateTask->addStartTask(m_tasks[2]);
    updateTask->addStartTask(m_tasks[3]);
    updateTask->addEndTask(m_tasks[1]);
    updateTask->addEndTask(m_tasks[2]);
    updateTask->addEndTask(m_tasks[3]);
}

Application::~Application(void)
{
}

int Application::run()
{
    m_tasks[0]->run(m_scheduler);
    m_scheduler->wait();
    return 0;
}

void Application::addSceneSync(ref<Graphics::Scene> scene, ref<Graphics::RenderTarget> target)
{
    RenderView view(scene, target);
    m_views.push_back(view);
    m_tasks[0]->addCallback(view.renderTask()->startCallback());
    view.renderTask()->addCallback(m_tasks[0]->startCallback());
}

void Application::addScene(ref<Graphics::Scene> scene, ref<Graphics::RenderTarget> target)
{
    Request* request = new Request();
    request->operation = Request::AddScene;
    request->param1 = scene;
    request->param2 = target;
    m_requests.push(request);
}

void Application::processRequests()
{
    while(Request* request = m_requests.pop())
    {
        switch(request->operation)
        {
        case Request::AddScene:
            addSceneSync(be_checked_cast<Graphics::Scene>(request->param1), be_checked_cast<Graphics::RenderTarget>(request->param2));
            break;
        case Request::RemoveScene:
        default:
            be_notreached();
            break;
        }
        delete request;
    }
}

}
