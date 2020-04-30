/* BugEngine <bugengine.devel@gmail.com> / 2008-2014
   see LICENSE for detail */

#include <bugengine/stdafx.h>

#include <bugengine/core/threads/thread.hh>
#include <bugengine/plugin/dynobjectlist.hh>

#include <android/log.h>
#include <jni.h>

BE_EXPORT const char* s_packagePath   = 0;
BE_EXPORT const char* s_dataDirectory = 0;
BE_EXPORT const char* s_plugin        = 0;

int beMain(int argc, const char* argv[]);

namespace BugEngine { namespace Android {

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
    virtual bool log(const BugEngine::istring& logname, BugEngine::LogLevel level, const char* filename, int line,
                     const char* thread, const char* msg) const
    {
        android_LogPriority s_bugengineToAndroid[logFatal + 1]
           = {ANDROID_LOG_VERBOSE, ANDROID_LOG_DEBUG, ANDROID_LOG_INFO,
              ANDROID_LOG_WARN,    ANDROID_LOG_ERROR, ANDROID_LOG_FATAL};
        android_LogPriority p = s_bugengineToAndroid[int(level)];
        __android_log_print(p, "BugEngine", "%s|%d: [%s:%s] %s", filename, line, thread, logname.c_str(), msg);
        return true;
    }
};

static intptr_t android_main(intptr_t /*width*/, intptr_t /*height*/)
{
    BugEngine::ScopedLogListener android_listener(scoped< AndroidLogListener >::create(BugEngine::Arena::debug()));
    BugEngine::Plugin::DynamicObjectList::showList();
    const char* args[] = {"BugEngine", s_plugin};
    return beMain(1 + (s_plugin != 0), args);
}

}}  // namespace BugEngine::Android

extern "C"
{
    JNIEXPORT void JNICALL Java_com_bugengine_BugEngineLib_setPaths(JNIEnv* env, jclass cls, jstring packagePath,
                                                                    jstring dataDirectory, jstring plugin);
    JNIEXPORT void JNICALL Java_com_bugengine_BugEngineLib_init(JNIEnv* env, jclass cls, jint width, jint height);
    JNIEXPORT void JNICALL Java_com_bugengine_BugEngineLib_step(JNIEnv* env, jclass cls);
}

static BugEngine::Thread* s_mainThread;
JNIEXPORT void JNICALL    Java_com_bugengine_BugEngineLib_setPaths(JNIEnv* env, jclass /*cls*/, jstring packagePath,
                                                                   jstring dataDirectory, jstring plugin)
{
    s_packagePath   = env->GetStringUTFChars(packagePath, 0);
    s_dataDirectory = env->GetStringUTFChars(dataDirectory, 0);
    s_plugin        = env->GetStringUTFChars(plugin, 0);
}

JNIEXPORT void JNICALL Java_com_bugengine_BugEngineLib_init(JNIEnv* /*env*/, jclass /*cls*/, jint width, jint height)
{
    s_mainThread = new BugEngine::Thread("android_main", &BugEngine::Android::android_main, width, height);
}

JNIEXPORT void JNICALL Java_com_bugengine_BugEngineLib_step(JNIEnv* /*env*/, jclass /*cls*/)
{
}
