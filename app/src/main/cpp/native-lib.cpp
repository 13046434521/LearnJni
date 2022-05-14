#include <jni.h>
#include <string>
#include "log.h"
//#if defined(__cplusplus)
//typedef _JNIEnv JNIEnv;
//typedef _JavaVM JavaVM;
//#else
//typedef const struct JNINativeInterface* JNIEnv;
//typedef const struct JNIInvokeInterface* JavaVM;
//#endif
//extern "C" 主要作用就是为了能够正确实现C++代码调用其他C语言代码。加上 extern “c” 后，会指示编译器这部分的代码按C语言，而不是C++的方式进行编译。
// C没有重载，而C++有重载。同样一个方法void fun(int,int) 在C中编译完可能对应的就是_fun,而C++中可能就是_fun_int_int，会把参数带上。
// JNIEXPORT和JNICALL是jni中定义的两个宏，使用JNIEXPORT支持在外部程序代码中调用该动态库中的方法，
// 使用JNICALL定义函数调用时参数的入栈出栈约定函数名称由包名+类名+方法名组成，
// 在该方法中有两个参数，通过第一个参数JNIEnv *的对象可以调用jni.h中封装好的大量函数 ，
// 第二个参数代表着native方法的调用者，当java代码中定义的native方法是静态方法时这里的参数是jclass，非静态方法的参数是jobject
//
//C/C++代码 编译成so库还是dll还是jnilib 是由C/C++的编译器来决定的。不同的编译器会编译出不同平台上的机器码。主要还是架构的区别。Windows运行的系统是X86架构的，而且PC上的CPU也是X86架构的，所以机器码就需要是X86架构的
extern "C" // 下面的代码采用C的编译方式。因为JNIEnv结构体，所有的函数都是C的方式写的。
            // 最终都是调用的JNINativeInterface结构体中的函数（300多个函数）
 JNIEXPORT //标记该方法可以被外部调用.如果不加，Java层, 外部无法调用该方法(Windows系统会报错，但是Linux仍然可以正常调用)
jstring // 返回值
JNICALL //约定函数的入栈顺序，和特定的堆栈的内存清理规则
// 非静态：Java_包名_类名_方法名(JNIEnv* env,jobject thiz) :thiz 指的是这个类的对象本身
// 静态：Java_包名_类名_方法名(JNIEnv* env,jclass clazz) : clazz 指的是这个类
// 如果包名本身有下划线，在下划线后面填加数字1，用来区分
Java_com_android_learn_1jni_Day1Activity_stringFromJNI(
        JNIEnv* env,
        jobject thiz /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_android_learn_1jni_Day1Activity_changeNameByCpp(JNIEnv *env, jobject thiz) {
    // 两种获取类的方法
    // 1. 直接通过全类名，来获取Class
    // 2. 通过传进来的对象来获取。这里是this
    jclass clazz1 =env->FindClass("com/android/learn_jni/Day1Activity");
    jclass clazz2 = env->GetObjectClass(thiz);

    // 获取要修改的对象的属性:String 的全类名是Ljava/lang/String; 这里的L代表的是引用类型
    // 签名Sig：void:V, boolean:Z，byte:B，char:C，short:S，int:I，float:F，long:J，double:D，引用类型:L+全类名;
    jfieldID  jfieldId = env->GetFieldID(clazz1,"name", "Ljava/lang/String;");
    jstring name = env->NewStringUTF("Jtl 学习 JNI");

    // 修改对象属性： env->SetObjectField(对象,属性,修改值);
    env->SetObjectField(thiz,jfieldId,name);

    // 回收创建的对象

    env->DeleteLocalRef(name);
    env->DeleteLocalRef(clazz1);
    env->DeleteLocalRef(clazz2);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_android_learn_1jni_Day1Activity_changeAgeByCpp(JNIEnv *env, jclass clazz) {
    jfieldID  jfieldId = env->GetStaticFieldID(clazz,"age", "I");

    // jboolean: unsigned char 1字节 8位
    // jbyte: signed char 1字节 8位
    // jchar: unsigned short 2字节 16位
    // jshort: short  2字节 16位
    // jint: int  4字节 32位
    // jfloat: float 4字节  32位
    // jlong: long  8字节 64位
    // jdouble: double 8字节 64位
    // jsize: int 4字节 32位
    jint age = env->GetStaticIntField(clazz,jfieldId);
    env->SetStaticIntField(clazz,jfieldId,age+9);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_android_learn_1jni_Day1Activity_changeHeightByCpp(JNIEnv *env, jobject thiz) {
    jclass jclazz = env->GetObjectClass(thiz);
    jfieldID jfieldId = env->GetFieldID(jclazz,"height","D");

    jdouble finalDouble = env->GetDoubleField(thiz,jfieldId);
    LOGD("C++ 中的 final height值修改前：%lf",finalDouble)
    env->SetDoubleField(thiz,jfieldId,174.0);
    jfieldID jfieldId1 = env->GetFieldID(jclazz,"height","D");
    jdouble finalDouble1= env->GetDoubleField(thiz,jfieldId1);
    LOGD("C++ 中的 final height值修改后：%lf",finalDouble1)
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_android_learn_1jni_Day1Activity_getHeightByCpp(JNIEnv *env, jobject thiz) {
    jclass jclass1 = env->FindClass("com/android/learn_jni/Day1Activity");
    jfieldID  jfieldId = env->GetFieldID(jclass1,"height", "D");
    jdouble  jdouble1 = env->GetDoubleField(thiz,jfieldId);

    LOGW("C++ 中的 final height：%lf",jdouble1)
    return jdouble1;
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_android_learn_1jni_Day1Activity_callAddMethod(JNIEnv *env, jobject thiz) {
    jclass clazz = env->GetObjectClass(thiz);
    //public void add(int i,String j) 只有引用类型最后用;号结尾，基本类型不用
    jmethodID  jmethodId = env->GetMethodID(clazz,"add","(ILjava/lang/String;)I");
    jint args1 = 95;
    jstring args = env->NewStringUTF("C++里传来的第一个参数是95");

    jint data = env->CallIntMethod(thiz,jmethodId,args1,args);
    LOGW("C++ 中在Java方法中获取到的值%d",data)

    return data;
}