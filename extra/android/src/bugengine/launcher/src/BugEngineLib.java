package com.bugengine;

import android.content.res.AssetManager;

public class BugEngineLib {

    static
    {
        System.loadLibrary("dl");
        System.loadLibrary("m");
        System.loadLibrary("log");
        System.loadLibrary("c++_shared");
        System.loadLibrary("bugengine");
     }

    /**
     * @param packagePath path to the APK
     */
     public static native void setPaths(String packagePath, String dataDirectory, String plugin);
    /**
     * @param width the current view width
     * @param height the current view height
     */
     public static native void init(int width, int height);
     public static native void step();
}

