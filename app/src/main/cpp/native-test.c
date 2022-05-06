//
// Created by jiatianlong on 2022/5/6.
//

#include <jni.h>

// C中写JNI
// C中的JNIEnv*env 是一个结构体指针
// 而 JNIEnv本身就是一个结构体指针：typedef const struct JNINativeInterface* JNIEnv;
// 即 JNIEnv* env是一个二级指针，需要先*env  获取他的一级指针，才能调用结构体指针JNIEnv的方法
// 而且C和C++的区别是。C++有对象的概念，C中没有。所以C中的方法比C++中的方法要多传一个env给函数
JNIEXPORT
jstring
JNICALL
Java_com_android_learn_1jni_MainActivity_changeName(JNIEnv *env,jobject jobject1){
    jstring  name = (*env)->NewStringUTF(env,"JTL is best");

    return name;
}