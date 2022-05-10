package com.android.learn_jni;

import androidx.appcompat.app.AppCompatActivity;

import android.app.Application;
import android.os.Bundle;
import android.util.Log;

public class Day2Activity extends AppCompatActivity {
    static {
        System.loadLibrary("learnjni");
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_day2);
        int [] data = new int[]{99,20,120,256,88};
        String[] array = new String[]{"C++","Java","Kotlin","Python"};

        tryArray(10,"贾天龙",data,array);

        for (int i=0;i<data.length;i++){
            Log.d("LearnJniCpp","java打印:int 数组：第"+i+"个元素:"+data[i]);
        }
    }




    public native void tryArray(int count ,String msg,int[] data,String[] array);
}