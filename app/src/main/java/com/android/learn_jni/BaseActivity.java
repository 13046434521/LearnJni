package com.android.learn_jni;

import androidx.appcompat.app.AppCompatActivity;

/**
 * @author：TianLong
 * @date：2022/5/18 14:36
 * @detail：
 */
class BaseActivity extends AppCompatActivity {
    static {
        System.loadLibrary("learnjni");
    }
    public static String TAG =Constants.TAG;
}
