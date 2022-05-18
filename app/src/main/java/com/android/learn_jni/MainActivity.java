package com.android.learn_jni;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;

import static com.android.learn_jni.Constants.datas;

public class MainActivity extends BaseActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        LinearLayout layout = findViewById(R.id.layout_main_linearlayout);

        for (int i=0;i<datas.length;i++){
            Button button = new Button(this);
            button.setText("day:"+(i+1));
            layout.addView(button);

            int finalI = i;
            button.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    Intent intent = new Intent(MainActivity.this,datas[finalI]);
                    startActivity(intent);
                }
            });
        }
    }
}