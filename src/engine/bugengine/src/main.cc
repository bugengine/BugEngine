/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <bugengine/stdafx.h>
#include    <bugengine/application.hh>
#include    <bugengine/main.hh>

#include    <core/environment.hh>
#include    <filesystem/diskfolder.script.hh>
#include    <scheduler/scheduler.hh>
#include    <resource/resourcemanager.hh>

#include    <plugin/plugin.hh>

#include    <cstdio>
#include    <cstdlib>
#include    <unistd.h>

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
        virtual bool log(const BugEngine::istring& logname, BugEngine::LogLevel level,
                         const char *filename, int line, const char* thread, const char *msg) const
        {
            const minitl::format<1024u>& message = minitl::format<1024u>("%s:%d (%s)\t(%s:%s) %s\n")
                    | filename | line | logname.c_str() | s_logNames[level] | thread | msg;
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
        virtual bool log(const BugEngine::istring& logname, BugEngine::LogLevel level,
                         const char *filename, int line,
                         const char* thread, const char *msg) const
        {
#ifdef BE_PLATFORM_WIN32
            minitl::format<1024u> message = minitl::format<1024u>("%s(%d): %s\t(%s) %s%s")
                                          | filename
                                          | line
                                          | logname.c_str()
                                          | s_logNames[level]
                                          | msg
                                          | (msg[strlen(msg)-1] == '\n' ? "" : "\n");
            OutputDebugString(message);
#endif
            static const char* term = BugEngine::Environment::getEnvironment().getEnvironmentVariable("TERM");
            static const char* colors[] = {
                isatty(1) && term ? "\x1b[0m" : "",
                isatty(1) && term ? "\x1b[01;1m" : "",
                isatty(1) && term ? "\x1b[36m" : "",
                isatty(1) && term ? "\x1b[32m" : "",
                isatty(1) && term ? "\x1b[33m" : "",
                isatty(1) && term ? "\x1b[31m" : "",
                isatty(1) && term ? "\x1b[35m" : ""
            };
            const char* color = colors[0];
            switch(level)
            {
            case BugEngine::logDebug:
                color = colors[2];
                break;
            case BugEngine::logInfo:
                color = colors[3];
                break;
            case BugEngine::logWarning:
                color = colors[4];
                break;
            case BugEngine::logError:
                color = colors[5];
                break;
            case BugEngine::logFatal:
                color = colors[6];
                break;
            case BugEngine::logSpam:
            default:
                break;
            }

            const char* normal = colors[0];
            fprintf(stdout, "[%s%s%s] %s%s(%s)%s: %s",
                    color, s_logNames[level], normal,
                    colors[1], logname.c_str(), thread, normal,
                    //filename, line,
                    msg);
            be_forceuse(filename);
            be_forceuse(line);
            if (msg[strlen(msg)-1] != '\n')
                fprintf(stdout, "\n");
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
        ref<BugEngine::DiskFolder> root = ref<BugEngine::DiskFolder>::create(
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
