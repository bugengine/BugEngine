/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <bugengine/stdafx.h>
#include    <bugengine/application.hh>
#include    <bugengine/main.hh>
#include    <rtti/engine/namespace.hh>

#include    <core/environment.hh>
#include    <filesystem/diskfolder.script.hh>
#include    <scheduler/scheduler.hh>
#include    <resource/resourcemanager.hh>
#include    <settings/providers/commandline.hh>
#include    <settings.script.hh>

#include    <plugin/plugin.hh>

#include    <cstdio>
#include    <cstdlib>
#include    <unistd.h>

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
            if (BugEngine::MainSettings::Log::get().enableFileLog)
            {
                const minitl::format<1024u>& message = minitl::format<1024u>("%s:%d (%s)\t(%s:%s) %s\n")
                        | filename | line | logname.c_str() | getLogLevelName(level) | thread | msg;
                m_logFile->beginWrite(message.c_str(), be_checked_numcast<u32>(strlen(message.c_str())));
            }
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
            if (BugEngine::MainSettings::Log::get().enableConsoleLog)
            {
                using namespace BugEngine;
#ifdef BE_PLATFORM_WIN32
                minitl::format<1024u> message = minitl::format<1024u>("%s(%d): %s\t(%s) %s%s")
                                              | filename
                                              | line
                                              | logname.c_str()
                                              | getLogLevelName(level)
                                              | msg
                                              | (msg[strlen(msg)-1] == '\n' ? "" : "\n");
                OutputDebugString(message);
# define isatty(x) 1
#endif
                static const char* term = Environment::getEnvironment().getEnvironmentVariable("TERM");
                static const char* colors[] = {
                    isatty(1) && term ? "\x1b[0m" : "",
                    isatty(1) && term ? "\x1b[01;1m" : "",
                    isatty(1) && term ? "\x1b[36m" : "",
                    isatty(1) && term ? "\x1b[32m" : "",
                    isatty(1) && term ? "\x1b[33m" : "",
                    isatty(1) && term ? "\x1b[31m" : "",
                    isatty(1) && term ? "\x1b[1;31m" : ""
                };
#ifdef BE_PLATFORM_WIN32
# undef isatty
#endif
                const char* color = colors[0];
                switch(level)
                {
                case logDebug:
                    color = colors[2];
                    break;
                case logInfo:
                    color = colors[3];
                    break;
                case logWarning:
                    color = colors[4];
                    break;
                case logError:
                    color = colors[5];
                    break;
                case logFatal:
                    color = colors[6];
                    break;
                case logSpam:
                default:
                    break;
                }

                const char* normal = colors[0];
                fprintf(stdout, "[%s%s%s] %s%s(%s)%s %s(%d): %s",
                        color, getLogLevelName(level), normal,
                        colors[1], logname.c_str(), thread, normal,
                        filename, line,
                        msg);
                fflush(stdout);
                be_forceuse(filename);
                be_forceuse(line);
                if (msg[strlen(msg)-1] != '\n')
                    fprintf(stdout, "\n");
            }
            return true;
        }
    };
}


int beMain(int argc, const char *argv[])
{
    using namespace BugEngine;
    Environment::getEnvironment().init(argc, argv);
#if BE_ENABLE_EXCEPTIONS
    try
#endif
    {
        ref<DiskFolder> root = ref<DiskFolder>::create(
                Arena::general(),
                Environment::getEnvironment().getHomeDirectory(),
                DiskFolder::ScanRecursive,
                DiskFolder::CreateOne);
        ref<DiskFolder> home = ref<DiskFolder>::create(
                Arena::general(),
                Environment::getEnvironment().getGameHomeDirectory(),
                DiskFolder::ScanRecursive,
                DiskFolder::CreateOne);
        Settings::CommandLineSettingsProvider settings(argc, argv, home);
        ScopedLogListener console(scoped<ConsoleLogListener>::create(Arena::debug()));
        Plugin::Plugin<minitl::pointer> platformAssert(
                inamespace("plugin.debug.assert"),
                Plugin::Context(weak<Resource::ResourceManager>(), ref<Folder>(), weak<Scheduler>()));
        ScopedLogListener file(scoped<FileLogListener>::create(Arena::debug(), home->createFile("log")));
        scoped<Scheduler> scheduler = scoped<Scheduler>::create(Arena::task());
        scoped<Resource::ResourceManager> manager = scoped<Resource::ResourceManager>::create(Arena::resource());;
        Plugin::Plugin<Application> app(
                inamespace(Environment::getEnvironment().getGame()),
                Plugin::Context(manager, home, scheduler));
        if (app)
        {
            be_info("Running %s" | Environment::getEnvironment().getGame());
            return app->run();
        }
        else
        {
            be_error("Failed to load main module \"%s\", exiting" | Environment::getEnvironment().getGame());
            return EXIT_FAILURE;
        }
    }
#if BE_ENABLE_EXCEPTIONS
    catch(...)
    {
        return EXIT_FAILURE;
    }
#endif
}
