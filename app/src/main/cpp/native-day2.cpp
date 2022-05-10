//
// Created by jiatianlong on 2022/5/10.
// 数组的修改必须通过env->Release相应的函数，才能把值传递给JAVA层中，否则更新无效

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
    }
    env->ReleaseIntArrayElements(data,int_array,JNI_OK);
}