#include <bugengine/plugin.scripting.pythonlib/stdafx.h>
#include <bugengine/core/environment.hh>
#include <bugengine/core/logger.hh>
#include <bugengine/plugin.scripting.pythonlib/pythonlib.hh>
#include <unistd.h>

class ConsoleLogListener : public BugEngine::ILogListener
{
private:
    minitl::AssertionCallback_t m_previousCallback;

public:
    ConsoleLogListener()
    {
        m_previousCallback = minitl::setAssertionCallback(&onAssert);
    }
    ~ConsoleLogListener()
    {
        minitl::setAssertionCallback(m_previousCallback);
    }

private:
    static minitl::AssertionResult onAssert(const char* file, int line, const char* expr,
                                            const char* message)
    {
        be_fatal("%s:%d Assertion failed: %s\n\t%s" | file | line | expr | message);
        return minitl::Break;
    }

protected:
    virtual bool log(const BugEngine::istring& logname, BugEngine::LogLevel level,
                     const char* filename, int line, const char* thread, const char* msg) const
    {
#ifdef BE_PLATFORM_WIN32
        minitl::format< 1024u > message
           = minitl::format< 1024u >("%s(%d): %s\t(%s) %s%s") | filename | line | logname.c_str()
             | getLogLevelName(level) | msg | (msg[strlen(msg) - 1] == '\n' ? "" : "\n");
        OutputDebugString(message);
#    define isatty(x) 1
#endif
        static const char* term
           = BugEngine::Environment::getEnvironment().getEnvironmentVariable("TERM");
        static const char* colors[]
           = {isatty(1) && term ? "\x1b[0m" : "",   isatty(1) && term ? "\x1b[01;1m" : "",
              isatty(1) && term ? "\x1b[36m" : "",  isatty(1) && term ? "\x1b[32m" : "",
              isatty(1) && term ? "\x1b[33m" : "",  isatty(1) && term ? "\x1b[31m" : "",
              isatty(1) && term ? "\x1b[1;31m" : ""};
#ifdef BE_PLATFORM_WIN32
#    undef isatty
#endif
        const char* color = colors[0];
        switch(level)
        {
        case BugEngine::logDebug: color = colors[2]; break;
        case BugEngine::logInfo: color = colors[3]; break;
        case BugEngine::logWarning: color = colors[4]; break;
        case BugEngine::logError: color = colors[5]; break;
        case BugEngine::logFatal: color = colors[6]; break;
        case BugEngine::logSpam:
        default: break;
        }

        const char* normal = colors[0];
        fprintf(stdout, "[%s%s%s] %s%s(%s)%s: %s", color, getLogLevelName(level), normal, colors[1],
                logname.c_str(), thread, normal,
                // filename, line,
                msg);
        fflush(stdout);
        be_forceuse(filename);
        be_forceuse(line);
        if(msg[strlen(msg) - 1] != '\n') fprintf(stdout, "\n");
        return true;
    }
};

BugEngine::ScopedLogListener
   console(scoped< ConsoleLogListener >::create(BugEngine::Arena::debug()));

extern "C" BE_EXPORT void initpy_bugengine()
{
    using namespace BugEngine;
    using namespace BugEngine::Python;
    /* python 2.x module initialisation */
    Environment::getEnvironment().init();
    be_info("loading module py_bugengine (Python 2)");
    static ref< PythonLibrary > s_loadedLibrary
       = ref< PythonLibrary >::create(Arena::general(), (const char*)0);
    setCurrentContext(s_loadedLibrary);
    init2_py_bugengine(false);
}

extern "C" BE_EXPORT BugEngine::Python::PyObject* PyInit_py_bugengine()
{
    using namespace BugEngine;
    using namespace BugEngine::Python;
    /* python 3.x module initialisation */
    Environment::getEnvironment().init();
    static ref< PythonLibrary > s_loadedLibrary
       = ref< PythonLibrary >::create(Arena::general(), (const char*)0);
    setCurrentContext(s_loadedLibrary);
    be_info("loading module py_bugengine (Python 3)");
    PyObject* module = init3_py_bugengine(false);
    return module;
}
