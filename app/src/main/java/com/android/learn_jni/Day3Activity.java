package com.android.learn_jni;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

public class Day3Activity extends BaseActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_day3);
    }

    public native void dynamicRegister();

    public native String dynamicRegister(int age);

    public native void nativeThread();

    public void dynamicRegister1(View view) {
        dynamicRegister();
    }

    public void dynamicRegister2(View view) {
        String aa = dynamicRegister(10);
        String msg = ((TextView) view).getText().toString();
        ((TextView) view).setText(msg + ":" + aa);
    }

    public void nativethread(View view) {
        nativeThread();
    }

    public void printThreadName(){
        String msg =  Thread.currentThread().getName();

        Log.e(TAG,msg);
    }
}