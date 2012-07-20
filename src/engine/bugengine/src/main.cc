/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <bugengine/stdafx.h>
#include    <bugengine/application.hh>

#include    <core/environment.hh>
#include    <core/threads/thread.hh>
#include    <system/file/diskfolder.script.hh>
#include    <system/plugin.hh>
#include    <system/scheduler/scheduler.hh>
#include    <system/resource/resourcemanager.hh>

#include    <cstdio>
#include    <cstdlib>

namespace
{
    class FileLogListener : public BugEngine::ILogListener
    {
    private:
        weak<BugEngine::File> m_logFile;
    public:
        explicit FileLogListener(weak<BugEngine::File> file)
            :   m_logFile(file)
        {
        }
        ~FileLogListener()
        {
        }
    protected:
        virtual bool log(const BugEngine::istring& logname, BugEngine::LogLevel level, const char *filename, int line, const char *msg) const
        {
            BugEngine::Debug::Format<4096> message = BugEngine::Debug::Format<4096>("%s:%d (%s)\t(%s) %s\n") | filename | line | logname.c_str() | s_logNames[level] | msg;
            m_logFile->beginWrite(message.c_str(), be_checked_numcast<u32>(strlen(message.c_str())));
            return true;
        }
    };

    class ConsoleLogListener : public BugEngine::ILogListener
    {
    public:
        ConsoleLogListener()
        {
        }
        ~ConsoleLogListener()
        {
        }
    protected:
        virtual bool log(const BugEngine::istring& logname, BugEngine::LogLevel level, const char *filename, int line, const char *msg) const
        {
#ifdef BE_PLATFORM_WIN32
            OutputDebugString(filename);
            OutputDebugString(BugEngine::Debug::Format<>("(%d) :") | line);
            OutputDebugString(logname.c_str());
            OutputDebugString("\t\t(");
            OutputDebugString(s_logNames[level]);
            OutputDebugString(") ");
            OutputDebugString(msg);
            if (msg[strlen(msg)-1] != '\n')
                OutputDebugString("\n");
#else
            fprintf(stderr, "%s(%d) :%s\t\t(%s) %s", filename, line, logname.c_str(), s_logNames[level], msg);
            if (msg[strlen(msg)-1] != '\n')
                fprintf(stderr, "\n");

#endif
            return true;
        }
    };
}

/*****************************************************************************/
static int beMain(int argc, const char *argv[])
{
    BugEngine::Environment::getEnvironment().init(argc, argv);
#if BE_ENABLE_EXCEPTIONS
    try
#endif
    {
        BugEngine::ScopedLogListener console(scoped<ConsoleLogListener>::create(BugEngine::Arena::debug()));
        ref<BugEngine::DiskFolder>::create(
                BugEngine::Arena::general(),
                BugEngine::Environment::getEnvironment().getHomeDirectory(),
                BugEngine::DiskFolder::ScanRecursive,
                BugEngine::DiskFolder::CreateOne);
        ref<BugEngine::DiskFolder> home = ref<BugEngine::DiskFolder>::create(
                BugEngine::Arena::general(),
                BugEngine::Environment::getEnvironment().getGameHomeDirectory(),
                BugEngine::DiskFolder::ScanRecursive,
                BugEngine::DiskFolder::CreateOne);
        BugEngine::ScopedLogListener file(scoped<FileLogListener>::create(BugEngine::Arena::debug(), home->createFile("log")));
        be_info("Running %s" | BugEngine::Environment::getEnvironment().getGame());
        scoped<BugEngine::Scheduler> scheduler = scoped<BugEngine::Scheduler>::create(BugEngine::Arena::task());
        BugEngine::Plugin<BugEngine::Application> app(
                BugEngine::inamespace(BugEngine::Environment::getEnvironment().getGame()),
                BugEngine::PluginContext(weak<BugEngine::ResourceManager>(), home, scheduler));
        return app->run();
    }
#if BE_ENABLE_EXCEPTIONS
    catch(...)
    {
        return EXIT_FAILURE;
    }
#endif
}
/*****************************************************************************/
#if defined(BE_PLATFORM_WIN32)

namespace BugEngine
{
    HINSTANCE hDllInstance;
}

extern "C"
int WINAPI WinMain( HINSTANCE hInstance,
                    HINSTANCE /*hPrevInstance*/,
                    LPSTR /*lpCmdLine*/,
                    int /*nCmdShow*/ )
{
    BugEngine::hDllInstance = hInstance;
    return beMain(__argc, (const char **)__argv);
}
#else
int main(int argc, const char *argv[])
{
    return beMain(argc, argv);
}
#endif
/*****************************************************************************/
