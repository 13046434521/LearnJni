//
// Created by jiatianlong on 2022/5/10.
// 数组的修改必须通过env->Release相应的函数，才能把值传递给JAVA层中，否则更新无效
// jclass clazz_activity = nullptr ;// 在JNI中这种也是局部变量
// 创建Java对象的两种方式： env->NewObject（会执行构造函数），env->AllocObject（不执行构造函数）
// Student(String name,int age)
// <init> 是构造函数。构造函数的返回签名必须是V

#include "jni.h"
#include "string"
#include "log.h"

void testString(JNIEnv* env){
    jstring temp = env->NewStringUTF("data");
    const char* data = env->GetStringUTFChars(temp,NULL);
    data = "111";
}

extern "C" // 为了让C++以C的方式编译，C没有重载，而C++中有重载。编译的时候，会把参数带上。
JNIEXPORT void JNICALL
Java_com_android_learn_1jni_Day2Activity_tryArray(JNIEnv *env, jobject thiz, jint count,
                                                  jstring msg, jintArray data, jobjectArray array) {
    int int_cpp = count;
    LOGW("c++打印：count:%d\n",int_cpp)

    const char* string_cpp = env->GetStringUTFChars(msg, NULL);
    LOGW("c++打印：msg:%s\n",string_cpp)
    env->ReleaseStringUTFChars(msg,string_cpp);


    jboolean  yes = 1;
    int* int_array= env->GetIntArrayElements(data, &yes);

    int int_length = env->GetArrayLength(data);
    for (int i = 0; i < int_length; ++i) {
        *(int_array+i) =  *(int_array+i)+100;
        LOGW("c++打印：int 数组：第%d个元素:%d   %p\n",i,*(int_array+i),int_array+i)
    }
    int array_length = env->GetArrayLength(array);
    for (int i = 0; i < array_length; ++i) {
        jstring string = (jstring)env->GetObjectArrayElement(array,i);
        const char* temp = env->GetStringUTFChars(string,NULL);
        LOGW("c++打印：修改前string 数组：第%d个元素:%s\n",i,temp)
        jstring data = env->NewStringUTF("测试");
        env->SetObjectArrayElement(array, i, (jobject) data);
        jstring string1 = (jstring)env->GetObjectArrayElement(array,i);
        const char* temp1 = env->GetStringUTFChars(string1,NULL);
        LOGW("c++打印：修改后string 数组：第%d个元素:%s\n",i,temp1)
        // 释放内存
        env->ReleaseStringUTFChars(string1,temp1);
    }
    // JNI_OK :对JAVA数组进行更新，释放内存空间
    // JNI_COMMIT :对JAVA数组进行更新，不释放内存空间
    // JNI_ABORT:不更新JAVA数组，释放内存空间
    env->ReleaseIntArrayElements(data,int_array,JNI_OK);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_android_learn_1jni_Day2Activity_changeStudent(JNIEnv *env, jobject thiz, jobject student) {
    jclass clazz = env->FindClass("com/android/learn_jni/Day2Activity$Student");
    jmethodID getName = env->GetMethodID(clazz,"getName", "()Ljava/lang/String;");
    jmethodID setName = env->GetMethodID(clazz,"setName", "(Ljava/lang/String;)V");

    jstring newName = env->NewStringUTF("张无忌");
    env->CallVoidMethod(student,setName,newName);
    env->DeleteLocalRef(newName);

    jstring name = (jstring) env->CallObjectMethod(student,getName );
    const char * name_c = env->GetStringUTFChars(name, NULL);
    LOGW("C++中name:%s",name_c)
    env->ReleaseStringUTFChars(name,name_c);

    jfieldID jfieldId = env->GetFieldID(clazz,"age", "I");

    int age_c = env->GetIntField(student,jfieldId);
    env->SetIntField(student,jfieldId,50);

    LOGW("C++中age:%d",age_c)
}
jclass clazz_activity = nullptr ;// 在JNI中这种也是局部变量
extern "C"
JNIEXPORT void
JNICALL
Java_com_android_learn_1jni_Day2Activity_newStudent(JNIEnv *env,jobject thiz){
    if (clazz_activity == nullptr){
        clazz_activity= env->GetObjectClass(thiz);
        env->NewGlobalRef(clazz_activity); // 将局部变量，改为全局变量。
    }
    jclass clazz_student = env->FindClass("com/android/learn_jni/Day2Activity$Student");
    // 创建Java对象的两种方式： env->NewObject（会执行构造函数），env->AllocObject（不执行构造函数）
    // Student(String name,int age)
    // <init> 是构造函数。构造函数的返回签名必须是V
    // 内部类的创建需要外部类的对象

    jmethodID jmethodId = env->GetMethodID(clazz_student,"<init>", "(Ljava/lang/String;I)V");
    jstring name = env->NewStringUTF("贾天龙");
    const char * name_char = env->GetStringUTFChars(name,NULL);

    jobject student_c = env->NewObject(clazz_student,jmethodId,name,29);
    LOGW("C++中创建对象：name:%s,age:%d",name_char,29)

    jmethodID tostring = env->GetMethodID(clazz_student,"toString","()Ljava/lang/String;");
    LOGW("C++中调用对象方法：tostring")
    env->CallObjectMethod(student_c,tostring);
    env->DeleteLocalRef(student_c);
}