package com.bugengine;

import android.app.Activity;
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

        BugEngineLib.initialize(getAssets());
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
