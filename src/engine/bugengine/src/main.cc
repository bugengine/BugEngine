/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <bugengine/stdafx.h>
#include    <bugengine/application.hh>

#include    <core/environment.hh>
#include    <filesystem/diskfolder.script.hh>
#include    <scheduler/scheduler.hh>
#include    <resource/resourcemanager.hh>

#include    <plugin/plugin.hh>

#include    <cstdio>
#include    <cstdlib>

#include    <minitl/hash_map.hh>
#include    <rtti/engine/array.hh>


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
            const minitl::format<1024u>& message = minitl::format<1024u>("%s:%d (%s)\t(%s) %s\n") | filename | line | logname.c_str() | s_logNames[level] | msg;
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
            OutputDebugString(minitl::format<1024u>("(%d) :") | line);
            OutputDebugString(logname.c_str());
            OutputDebugString("\t\t(");
            OutputDebugString(s_logNames[level]);
            OutputDebugString(") ");
            OutputDebugString(msg);
            if (msg[strlen(msg)-1] != '\n')
                OutputDebugString("\n");
#endif
            fprintf(stderr, "%s(%d) :%s\t\t(%s) %s", filename, line, logname.c_str(), s_logNames[level], msg);
            if (msg[strlen(msg)-1] != '\n')
                fprintf(stderr, "\n");
            return true;
        }
    };
}


int beMain(int argc, const char *argv[])
{
    BugEngine::Environment::getEnvironment().init(argc, argv);
#if BE_ENABLE_EXCEPTIONS
    try
#endif
    {
        BugEngine::ScopedLogListener console(scoped<ConsoleLogListener>::create(BugEngine::Arena::debug()));
        BugEngine::Plugin::Plugin<minitl::pointer> platformAssert(
                BugEngine::inamespace("plugin.debug.assert"),
                BugEngine::Plugin::Context(weak<BugEngine::Resource::ResourceManager>(), ref<BugEngine::Folder>(), weak<BugEngine::Scheduler>()));
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
        BugEngine::Plugin::Plugin<BugEngine::Application> app(
                BugEngine::inamespace(BugEngine::Environment::getEnvironment().getGame()),
                BugEngine::Plugin::Context(weak<BugEngine::Resource::ResourceManager>(), home, scheduler));
        return app->run();
    }
#if BE_ENABLE_EXCEPTIONS
    catch(...)
    {
        return EXIT_FAILURE;
    }
#endif
}
