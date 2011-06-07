/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <main/stdafx.h>
#include    <main/main.hh>
#include    <core/environment.hh>

#include    <cstdio>
#include    <cstdlib>

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
    try
    {
        BugEngine::Logger::root()->addListener(new LogListener("log.txt"));
        ref<BugEngine::Application> locApplication = ref<BugEngine::Application>::create(BugEngine::taskArena(), argc, argv);
        return be_main(locApplication);
    }
    catch(...)
    {
        return EXIT_FAILURE;
    }
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
