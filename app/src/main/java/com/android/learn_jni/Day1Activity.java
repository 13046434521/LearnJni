package com.android.learn_jni;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import com.android.learn_jni.databinding.ActivityMainBinding;


public class Day1Activity extends AppCompatActivity {



    private ActivityMainBinding binding;
    private String name  = "JAVA_NAME";
    private static int age = 20;
    private final double height = 180.00;
    private StringBuilder mStringBuilder = new StringBuilder();
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_day1);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());

        mStringBuilder.append(changeNameByC()+"\n");
        mStringBuilder.append("没修改的name:"+name+"\n");
        changeNameByCpp();
        mStringBuilder.append("修改过的name:"+name+"\n");
        mStringBuilder.append("修改前的age:"+age+"\n");
        changeAgeByCpp();
        mStringBuilder.append("修改后的age:"+age+"\n");

        mStringBuilder.append("修改前的final:height:"+height+"\n");
        Log.w("height","修改前的final:height:"+height);
        changeHeightByCpp();
        Log.w("height","修改后的final:height:"+height);
        mStringBuilder.append("修改后的final:height:"+height+"\n");
        tv.setText(mStringBuilder.toString());
        mStringBuilder.append("----------------------------------"+"\n");
        tv.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                double height1= getHeightByCpp();
                double a = height;
                mStringBuilder.append("Java中获取到的height:"+height+"\n");
                Log.w("height","Java中获取C++中获取到的height:"+height1);
                mStringBuilder.append("Java中获取C++中获取到的a = height:"+a+"\n");
                mStringBuilder.append("Java中获取C++中获取到的height:"+height1+"\n");
                mStringBuilder.append("--------------------\n");

                int data = callAddMethod();
                mStringBuilder.append("Java代码中C++方法返回的add方法的值:"+data+"\n");
                mStringBuilder.append("--------------------\n");
                tv.setText(mStringBuilder.toString());
            }
        });
    }

    /**
     * A native method that is implemented by the 'le' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native void changeNameByCpp();

    public native static void changeAgeByCpp();

    public native static String changeNameByC();

    public native void changeHeightByCpp();

    public native int getHeightByCpp();

    public native int callAddMethod();

    public int add(int i,String j){
        int total = (int)(i+height);
        Log.w("total","现在是Java代码:"+j+":total的值是："+total);
        return total;
    }
}