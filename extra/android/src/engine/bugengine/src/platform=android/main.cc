/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include    <bugengine/stdafx.h>

#include    <jni.h>
#include    <core/threads/thread.hh>
#include    <android/log.h>

BE_EXPORT const char* s_packagePath = 0;
BE_EXPORT const char* s_dataDirectory = 0;

int beMain(int argc, const char *argv[]);

namespace BugEngine { namespace Android
{

class AndroidLogListener : public BugEngine::ILogListener
{

public:
    AndroidLogListener()
    {
    }

    ~AndroidLogListener()
    {
    }
protected:
    virtual bool log(const BugEngine::istring& logname, BugEngine::LogLevel level, const char *filename, int line, const char *msg) const
    {
        android_LogPriority s_bugengineToAndroid[logFatal+1] = {
                ANDROID_LOG_VERBOSE,
                ANDROID_LOG_DEBUG,
                ANDROID_LOG_INFO,
                ANDROID_LOG_WARN,
                ANDROID_LOG_ERROR,
                ANDROID_LOG_FATAL
            };
        android_LogPriority p = s_bugengineToAndroid[int(level)];
        __android_log_print(p, "BugEngine", "%s|%d: [%s] %s", filename, line, logname.c_str(), msg);        
        return true;
    }
};

static intptr_t android_main(intptr_t /*width*/, intptr_t /*height*/)
{
    BugEngine::ScopedLogListener android_listener(scoped<AndroidLogListener>::create(BugEngine::Arena::debug()));
    const char *args[] = { "BugEngine" };
    return beMain(1, args);
}

}}

extern "C"
{
    JNIEXPORT void JNICALL Java_com_bugengine_BugEngineLib_setPaths(JNIEnv* env, jclass cls, jstring packagePath, jstring dataDirectory);
    JNIEXPORT void JNICALL Java_com_bugengine_BugEngineLib_init(JNIEnv* env, jclass cls, jint width, jint height);
    JNIEXPORT void JNICALL Java_com_bugengine_BugEngineLib_step(JNIEnv* env, jclass cls);
}

static BugEngine::Thread* s_mainThread;
JNIEXPORT void JNICALL Java_com_bugengine_BugEngineLib_setPaths(JNIEnv* env, jclass /*cls*/, jstring packagePath, jstring dataDirectory)
{
    s_packagePath = env->GetStringUTFChars(packagePath, 0);
    s_dataDirectory = env->GetStringUTFChars(dataDirectory, 0);
}

JNIEXPORT void JNICALL Java_com_bugengine_BugEngineLib_init(JNIEnv* /*env*/, jclass /*cls*/, jint width, jint height)
{
    s_mainThread = new BugEngine::Thread("android_main", &BugEngine::Android::android_main, width, height);
}

JNIEXPORT void JNICALL Java_com_bugengine_BugEngineLib_step(JNIEnv* /*env*/, jclass /*cls*/)
{
}
