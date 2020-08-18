package com.bugengine;

import android.app.Activity;
import android.content.pm.PackageManager;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.util.Log;
import android.view.WindowManager;

import java.io.File;


public class BugEngineActivity extends Activity
{
    BugEngineView mView;

    @Override protected void onCreate(Bundle icicle)
    {
        super.onCreate(icicle);
        String plugin = "";
        try
        {
            ActivityInfo ai = getPackageManager().getActivityInfo(this.getComponentName(), PackageManager.GET_META_DATA);
            Bundle metaData = ai.metaData;
            plugin = metaData.getString("app", "");
        }
        catch(PackageManager.NameNotFoundException e)
        {
        }
        String apkPath = getApplicationInfo().sourceDir;
        String dataPath = getApplicationInfo().dataDir;
        Log.i("BugEngine", "Running bugengine app " + plugin);
        BugEngineLib.setPaths(apkPath, dataPath, plugin);
        mView = new BugEngineView(getApplication());
        setContentView(mView);
    }

    @Override protected void onPause()
    {
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume()
    {
        super.onResume();
        mView.onResume();
    }
}
