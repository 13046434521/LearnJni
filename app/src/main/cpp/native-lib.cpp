#include <jni.h>
#include <string>
//#if defined(__cplusplus)
//typedef _JNIEnv JNIEnv;
//typedef _JavaVM JavaVM;
//#else
//typedef const struct JNINativeInterface* JNIEnv;
//typedef const struct JNIInvokeInterface* JavaVM;
//#endif
//extern "C"告诉编译器，这部分代码使用C语言规则来进行编译
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
Java_com_android_learn_1jni_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject thiz /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_android_learn_1jni_MainActivity_staticTest(JNIEnv *env, jclass clazz) {
    // TODO: implement changeName()
}