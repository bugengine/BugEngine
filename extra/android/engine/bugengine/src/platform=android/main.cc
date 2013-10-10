/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>
   see LICENSE for detail */

#include    <bugengine/stdafx.h>

#include    <jni.h>
#include    <core/threads/thread.hh>
#include    <android/log.h>
#include    <android/asset_manager.h>
#include    <android/asset_manager_jni.h>

#define  LOG_TAG    "BugEngine"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

BE_EXPORT AAssetManager* s_assetManager = 0;

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
    JNIEXPORT void JNICALL Java_com_bugengine_BugEngineLib_initialize(JNIEnv* env, jobject obj, jobject assetManager);
    JNIEXPORT void JNICALL Java_com_bugengine_BugEngineLib_init(JNIEnv* env, jobject obj, jint width, jint height);
    JNIEXPORT void JNICALL Java_com_bugengine_BugEngineLib_step(JNIEnv* env, jobject obj);
}

static BugEngine::Thread* s_mainThread;
JNIEXPORT void JNICALL Java_com_bugengine_BugEngineLib_initialize(JNIEnv* env, jobject /*obj*/,  jobject assetManager)
{
    s_assetManager = AAssetManager_fromJava(env, assetManager);
}

JNIEXPORT void JNICALL Java_com_bugengine_BugEngineLib_init(JNIEnv* /*env*/, jobject /*obj*/,  jint width, jint height)
{
    s_mainThread = new BugEngine::Thread("android_main", &BugEngine::Android::android_main, width, height);
}

JNIEXPORT void JNICALL Java_com_bugengine_BugEngineLib_step(JNIEnv* /*env*/, jobject /*obj*/)
{
}
