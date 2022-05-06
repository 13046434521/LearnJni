package com.android.learn_jni;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import com.android.learn_jni.databinding.ActivityMainBinding;


public class MainActivity extends AppCompatActivity {

    // Used to load the 'le' library on application startup.
    static {
        System.loadLibrary("learnjni");
    }

    private ActivityMainBinding binding;
    private String name  = "JAVA_NAME";
    private StringBuilder mStringBuilder = new StringBuilder();
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());

        mStringBuilder.append(changeNameByC()+"\n");
        mStringBuilder.append("没修改的name:"+name+"\n");
        changeNameByCpp();
        mStringBuilder.append("修改过的name:"+name+"\n");
        tv.setText(mStringBuilder.toString());
    }

    /**
     * A native method that is implemented by the 'le' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native void changeNameByCpp();

    public native static String changeNameByC();
}