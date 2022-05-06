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


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());
        tv.setText(changeName());
    }

    /**
     * A native method that is implemented by the 'le' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native static String staticTest();

    public native static String changeName();
}