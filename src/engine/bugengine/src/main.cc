/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <bugengine/stdafx.h>
#include    <core/environment.hh>
#include    <system/file/diskfolder.script.hh>

#include    <cstdio>
#include    <cstdlib>

namespace
{
    /*class LogListener : public BugEngine::ILogListener
    {
    private:
        FILE* m_logFile;
    public:
        LogListener(const char *logname)
            :   m_logFile(fopen(logname, "w"))
        {
        }
        ~LogListener()
        {
            fclose(m_logFile);
        }
    protected:
        virtual bool log(const BugEngine::istring& logname, BugEngine::LogLevel level, const char *filename, int line, const char *msg) throw()
        {
            fprintf(m_logFile, "%s:%d (%s)"
                               "\t(%s) %s\n", filename, line, logname.c_str(), s_logNames[level], msg);
            fflush(m_logFile);
            return true;
        }
    };*/

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
        virtual bool log(const BugEngine::istring& logname, BugEngine::LogLevel level, const char *filename, int line, const char *msg) throw()
        {
#ifdef BE_PLATFORM_WIN32
            OutputDebugString(filename);
            OutputDebugString(minitl::format<>("(%d) :") | line);
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
static int __main(int argc, const char *argv[])
{
    BugEngine::Environment::getEnvironment().init(argc, argv);
#ifdef BE_ENABLE_EXCEPTIONS
    try
#endif
    {
        BugEngine::Logger::root()->addListener(ref<ConsoleLogListener>::create(BugEngine::debugArena()));
        ref<BugEngine::DiskFolder> home = ref<BugEngine::DiskFolder>::create(
                BugEngine::gameArena(),
                BugEngine::Environment::getEnvironment().getGameHomeDirectory(),
                BugEngine::DiskFolder::ScanRecursive,
                BugEngine::DiskFolder::CreateOne);
        //ref<File> f = home->createFile("log")
        //BugEngine::Logger::root()->addListener(new LogListener(home));
        be_info("Running %s" | BugEngine::Environment::getEnvironment().getGame());
        ref<BugEngine::Application> locApplication = ref<BugEngine::Application>::create(BugEngine::taskArena(), argc, argv);
        BugEngine::Plugin<void*> plugin(BugEngine::Environment::getEnvironment().getGame(), weak<BugEngine::Application>(locApplication));
        return locApplication->run();
    }
#ifdef BE_ENABLE_EXCEPTIONS
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
    int result = __main(__argc, (const char **)__argv);
    return result;
}
#else
int main(int argc, const char *argv[])
{
    return __main(argc, argv);
}
#endif
/*****************************************************************************/
