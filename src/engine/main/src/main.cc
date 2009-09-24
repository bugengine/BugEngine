/* BugEngine / Copyright (C) 2005-2009  screetch <screetch@gmail.com>
   see LICENSE for detail */

#include    <main/stdafx.h>
#include    <main/main.hh>
#include    <rtti/namespace.hh>
#include    <rtti/test.hh>
#include    <rtti/autoregistration.hh>
#include    <rtti/mono.hh>
#include    <system/filesystem.hh>
#include    <system/diskfs.hh>
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

struct NamespaceScope
{
    NamespaceScope()  { }
    ~NamespaceScope() { BugEngine::RTTI::Namespace::root()->clear(); }
};

namespace
{
    class Environment : public BugEngine::Object
    {
        be_metaclass(NONE,Environment,Object)
            const BugEngine::ipath& getHomeDirectory() { return BugEngine::Environment::getEnvironment().getHomeDirectory(); }
            const BugEngine::ipath& getDataDirectory() { return BugEngine::Environment::getEnvironment().getDataDirectory(); }
            const BugEngine::ipath& getPluginDirectory() { return BugEngine::Environment::getEnvironment().getPluginDirectory(); }
            const BugEngine::istring& getGame() { return BugEngine::Environment::getEnvironment().getGame(); }
            const BugEngine::istring& getUser() { return BugEngine::Environment::getEnvironment().getUser(); }
            size_t getProcessorCount() { return BugEngine::Environment::getEnvironment().getProcessorCount(); }
        be_properties
            be_classmethod(getHomeDirectory);
            be_classmethod(getDataDirectory);
            be_classmethod(getPluginDirectory);
            be_classmethod(getGame);
            be_classmethod(getUser);
            be_classmethod(getProcessorCount);
        be_end
    };

    be_abstractmetaclass_impl("", Environment);


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
            fprintf(m_logFile, "%s:%d (%s)\n"
                               "\t(%s) %s\n", filename, line, logname.c_str(), s_logNames[level], msg);
            fflush(m_logFile);
            #ifdef _WIN32
                OutputDebugString(filename);
                OutputDebugString(minitl::format<>(":%d (") | line);
                OutputDebugString(logname.c_str());
                OutputDebugString(")\n");
                OutputDebugString("\t(");
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
#   ifdef _GP2X
    atexit(launch_gp2xmenu);
#   endif
    int result = EXIT_FAILURE;

    try
    {
        BugEngine::Logger::root()->addListener(new LogListener("log.txt"));
        BugEngine::FileSystem::instance()->mount("data", new BugEngine::DiskFS(BugEngine::Environment::getEnvironment().getDataDirectory(), true));

        refptr<BugEngine::Application> locApplication = new BugEngine::Application(argc, argv);
        result = be_main(locApplication.get());
    }
    catch(std::runtime_error& er)
    {
        fprintf(stderr, "%s\n", er.what());
    }

    return result;
}
/*****************************************************************************/
#if defined(_WIN32)

namespace BugEngine
{
    HINSTANCE hDllInstance;
}

#define ARGC_MAX    4096

static void skipBackslash(char * &cmdLine)
{
    cmdLine++;
    if (*cmdLine) cmdLine++;
}

static void lookupNextQuote(char * &cmdLine)
{
    cmdLine++;

    while (*cmdLine && *cmdLine != '"')
    {
        if (*cmdLine == '\\')
            skipBackslash(cmdLine);
        else
            cmdLine++;
    }
    if (*cmdLine) cmdLine++;
}

static void parceCmdLine( char *cmdLine, int &argc, const char *argv[] )
{
    while(*cmdLine)
    {
        argv[argc++] = cmdLine;
        if (argc == ARGC_MAX)
        {
            return;
        }
        else
        {
            while(*cmdLine && (*(cmdLine) != ' '))
            {
                if (*cmdLine == '"')
                {
                    lookupNextQuote(cmdLine);
                }
                else if (*cmdLine == '\\')
                {
                    skipBackslash(cmdLine);
                }
                else
                    cmdLine++;
            }
            if (*cmdLine) *(cmdLine++) = 0;
        }
    }
}

extern "C"
int WINAPI WinMain( HINSTANCE hInstance,
                    HINSTANCE /*hPrevInstance*/,
                    LPSTR /*lpCmdLine*/,
                    int /*nCmdShow*/ )
{
    BugEngine::hDllInstance = hInstance;
    int argc = 0;
    const char *argv[ARGC_MAX];
    char *cmdLine = GetCommandLine();


    parceCmdLine(cmdLine, argc, argv);

    const char* path=argv[0];
    const char *lastComponent = 0;
    while(*path != '\0')
    {
        if(*path == '\\')
            lastComponent = path;
        path++;
    }
    if(lastComponent)
    {
        char backup = *lastComponent;
        *const_cast<char*>(lastComponent) = 0;
        SetCurrentDirectory(cmdLine);
        *const_cast<char*>(lastComponent) = backup;
    }
    freopen("be_out.txt", "w+", stdout);
    freopen("be_err.txt", "w+", stderr);


    int result = __main(argc, &(argv[0]));
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
