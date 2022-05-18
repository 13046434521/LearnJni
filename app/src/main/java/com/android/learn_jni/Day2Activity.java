package com.android.learn_jni;

import androidx.appcompat.app.AppCompatActivity;

import android.app.Application;
import android.os.Bundle;
import android.util.Log;

public class Day2Activity extends BaseActivity {

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
        for (int i=0;i<array.length;i++){
            Log.d("LearnJniCpp","java打印:String 数组：第"+i+"个元素:"+array[i]);
        }

        Log.w("LearnJniCpp","-------------------------------------------");
        Student student = new Student("贾天龙",29);

        changeStudent(student);

        Log.w("LearnJniCpp","Java层：修改过后的name:"+student.getName());
        Log.w("LearnJniCpp","Java层：修改过后的age:"+student.getAge());

        newStudent();
    }

    public native void tryArray(int count ,String msg,int[] data,String[] array);
    public native void changeStudent(Student student);
    public native void newStudent();

    static class Student{
        private String name;
        private int age;
        Student(){}
        Student(String name,int age){
            this.age = age;
            this.name = name;
        }
        Student(int age,String name){
            this.age = age;
            this.name = name;
        }

        public String getName() {
            return name;
        }

        public void setName(String name) {
            this.name = name;
        }

        public int getAge() {
            return age;
        }

        public void setAge(int age) {
            this.age = age;
        }

        @Override
        public String toString() {
            Log.w("LearnJniCpp","Java : Student{" +
                    "name='" + name + '\'' +
                    ", age=" + age +
                    '}');
            return "LearnJniCpp: Java : Student{" +
                    "name='" + name + '\'' +
                    ", age=" + age +
                    '}';
        }
    }
}