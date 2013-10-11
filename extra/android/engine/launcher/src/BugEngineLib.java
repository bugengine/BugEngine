package com.bugengine;

import android.content.res.AssetManager;

public class BugEngineLib {

     static
     {
         System.loadLibrary("engine.bugengine");
     }

    /**
     * @param packagePath path to the APK
     */
     public static native void initialize(String packagePath, String dataDirectory);
    /**
     * @param width the current view width
     * @param height the current view height
     */
     public static native void init(int width, int height);
     public static native void step();
}
