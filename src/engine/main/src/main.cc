/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <main/stdafx.h>
#include    <main/main.hh>
#include    <input/inputmap.hh>
#include    <system/scheduler/scheduler.hh>
#include    <core/environment.hh>

#include    <cstdio>
#include    <cstdlib>

#include    <core/memory/new.inl>

#ifdef _GP2X
#include    <unistd.h>
static void launch_gp2xmenu(void)
{
    printf("returning to menu...\n");
    char **args = { 0 };
    chdir("/usr/gp2x");
    execv("/usr/gp2x/gp2xmenu", args);
}
#endif

namespace
{
    class LogListener : public BugEngine::ILogListener
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
            #ifdef BE_PLATFORM_WIN32
                OutputDebugString(filename);
                OutputDebugString(minitl::format<>("(%d) :") | line);
                OutputDebugString(logname.c_str());
                OutputDebugString(")\t\t(");
                OutputDebugString(s_logNames[level]);
                OutputDebugString(") ");
                OutputDebugString(msg);
                OutputDebugString("\n");
            #endif
            return true;
        }
    };
}

/*****************************************************************************/
static int __main(int argc, const char *argv[])
{
	BugEngine::Environment::getEnvironment().init(argc, argv);
#   ifdef _GP2X
    atexit(launch_gp2xmenu);
#   endif
    int result = EXIT_FAILURE;

    try
    {
        BugEngine::Logger::root()->addListener(new LogListener("log.txt"));
        ref<BugEngine::Application> locApplication = ref<BugEngine::Application>::create<BugEngine::Arena::General>(argc, argv);
        result = be_main(locApplication);
    }
    catch(std::runtime_error& er)
    {
        fprintf(stderr, "%s\n", er.what());
    }

    return result;
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
    freopen("be_out.txt", "w+", stdout);
    freopen("be_err.txt", "w+", stderr);

    int result = __main(__argc, (const char **)__argv);
    fflush(stdout);
    fflush(stderr);

    return result;
}
#else
int main(int argc, const char *argv[])
{
    return __main(argc, argv);
}
#endif
/*****************************************************************************/
