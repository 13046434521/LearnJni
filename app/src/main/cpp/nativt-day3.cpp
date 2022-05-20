//
// Created by jiatianlong on 2022/5/18.
//Bad JNI version returned from JNI_OnLoad
// android 1.6-4.3 version >= JNI_VERSION_1_2合法
// android 4.4-7.1 version == JNI_VERSION_1_2 | JNI_VERSION_1_4 | JNI_VERSION_1_6
//
// 获取JavaVM常用的两种方法：
// 1.JNI_OnLoad中的JavaVM指针保存
// 2.env->GetJavaVM()
// 获取JNIEnv 的两种方法
// 1. jint res = vm->AttachCurrentThread(&env, nullptr);
// 2. jint res = vm->GetEnv((void **) &env, JNI_VERSION_1_6);
// 第一种方法获取的env只能FindClass只能通过系统类加载器进行加载
// 所以应用类加载器加载的自定义类无法加载。FindClass返回的class为null
// 第二种方法获取env的前提必须是线程已经被Attach到JVM上，否则GetEnv返回-2
#include <jni.h>
#include "log.h"
#include "pthread.h"
//typedef struct {
//    const char* name; // Java中的方法名
//    const char* signature; // 方法签名
//    void*       fnPtr; // C++中的方法名
//} JNINativeMethod;


// 动态注册的方法，自动包好JNIEnv*env,和jobject obj。你在方法中写或者不写都可以
// 签名，可以帮助区分，Java中重载的方法。
void dynamicRegister();

jstring dynamicRegister1(JNIEnv *env, jobject obj, jint age);


void nativeThread1(JNIEnv *env, jobject obj);

void nativeThread2(JNIEnv *env, jobject obj);

static JNINativeMethod methods[] = {
        {"dynamicRegister",                  "()V",                   (void *) dynamicRegister},
        {"dynamicRegister",                  "(I)Ljava/lang/String;", (jstring *) dynamicRegister1},
        {"nativeThread_AttachCurrentThread", "()V",                   (void *) nativeThread1},
        {"nativeThread_GetEnv",              "()V",                   (void *) nativeThread2}
};

// JNIEnv 不能跨线程，否则会崩溃（主线程的JNIEnv 传递到子线程中使用崩溃）
// jobject 不能跨线程，否则会崩溃（主线程的jobject 传递到子线程中使用崩溃）
// jobject 不能跨函数，否则会崩溃（函数A的jobject 传递到函数B中使用会崩溃）
// 一般用JavaVM解决JNIEnv跨线程这个问题
// JNIEnv 是绑定线程的，哪个线程调用，就属于哪个线程
JavaVM *vm;
jobject globleObject = nullptr;

jint JNI_OnLoad(JavaVM *vm, void *args) {
    ::vm = vm;
    JNIEnv *env = nullptr;

    //C 中 JNI_OK:0 就是返回成功。
    //二级指针的目的是给一级指针赋值
    //jint GetEnv(void** env, jint version)：这里需要二级指针，所以我需要对一级指针取址
    jint res = vm->GetEnv((void **) &env, JNI_VERSION_1_6);
    if (res != JNI_OK) {
        LOGD("JNI_OnLoad失败:%d", res)
        return JNI_FALSE;
    }
    LOGD("JNI_OnLoad成功:%d", res)
    jclass day3clazz = env->FindClass("com/android/learn_jni/Day3Activity");
    jsize size = sizeof methods / sizeof(JNINativeMethod);

    for (int i = 0; i < size; ++i) {
        JNINativeMethod *nativeMethod = methods + i;
        jint res_method = env->RegisterNatives(day3clazz, nativeMethod, 1);
        if (res_method != JNI_OK) {
            LOGW("注册方法:%s ,签名:%s,失败", nativeMethod->name, nativeMethod->signature)
        } else {
            LOGW("注册方法:%s,签名:%s,成功", nativeMethod->name, nativeMethod->signature)
        }
    }


    return JNI_VERSION_1_6;
}

void dynamicRegister() {
    LOGW("C++:调用dynamicRegister方法")
}

jstring dynamicRegister1(JNIEnv *env, jobject obj, jint age) {

    char *msg = "JNI_OnLoad";

    jstring dd = env->NewStringUTF(msg);

    LOGW("C++:获取到Java方法中传下来的age:%d", age)

    return dd;
}


// 返回值为void*的函数，必须要有返回值，一般为nullptr
// args参数是，pthread_create的第四个参数。第四个参数是什么，这里的args就是什么
// 子线程env->FindClass 失败原因：
// 通过AttachCurrentThread附加到虚拟机的线程在查找类时只会通过系统类加载器进行查找，
// 不会通过应用类加载器进行查找，因此可以加载系统类，但是不能加载非系统类，
// 如自己在java层定义的类会返回NULL

void *cppRunThread(void *args) {

    JNIEnv *env = nullptr;
    jint res = vm->AttachCurrentThread(&env, nullptr);
    if (res != JNI_OK) {
        LOGW("CPP:res is %d", res)
        return nullptr;
    }

    jclass clazz = env->GetObjectClass(globleObject);
    jmethodID jmethodId = env->GetMethodID(clazz, "printThreadName", "()V");
    env->CallVoidMethod(globleObject, jmethodId);

    vm->DetachCurrentThread();
    return nullptr;
}

// 由于没有Attach到JVM上，所以GetEnv获取env时，返回为-2，获取失败。
void* cppRunThread2(void*args){
    JNIEnv* env = nullptr;
    jint res = vm->GetEnv((void**)&env,JNI_VERSION_1_4);
    if (res!=JNI_OK){
        LOGW("RES %d",res)
        return nullptr;
    }

    jclass clazz = env->FindClass("com/android/learn_jni/Day3Activity");
    jmethodID jmethodId = env->GetMethodID(clazz,"printThreadName","()V");
    env->CallVoidMethod(globleObject,jmethodId);

    env->DeleteLocalRef(clazz);
    return nullptr;
}

void nativeThread1(JNIEnv *env, jobject obj) {
/*     int pthread_create(pthread_t* __pthread_ptr,
     pthread_attr_t const* __attr,
     void* (*__start_routine)(void*),
     void*);*/
//    int pthread_create(线程号,nullptr,函数指针(线程运行时的方法),函数指针里面的的参数)
    pthread_t tid = 199366;

    globleObject = env->NewGlobalRef(obj);
    int i = pthread_create(&tid, nullptr, cppRunThread, nullptr);
    if (i==JNI_OK){
        LOGW("线程创建成功")
    }
    LOGW("线程Id为：%ld",tid)
    pthread_join(tid , nullptr);
    LOGW("线程Id为：%ld",tid)
}

void nativeThread2(JNIEnv *env, jobject obj) {

    long dd = 199366;
    pthread_t *tid = &dd;
    globleObject = env->NewGlobalRef(obj);
    pthread_create(tid, nullptr, cppRunThread2, nullptr);
}