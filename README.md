# LearnJni
LearnJNI

### DAY1:JNI基础
#### 1.JNI方法名注意事项
    1. Jni中普通方法：extern "C" JNIEXPORT 返回类型 JNICALL Java_包名_类名_方法名(JNIEnv * env,jobject thiz){}
    2. Jni中静态方法：extern "C" JNIEXPORT 返回类型 JNICALL Java_包名_类名_方法名(JNIEnv * env,jclass clazz){}。如果包名本身有下划线，在下划线后面填加数字1，用来区分
    3. extern "C":为了能够正确实现C++代码调用其他C语言代码,让代码以C的方式进行编译。因为C中没有重载的概念。而C++中有重载的概念。
        假如一个方法 void fun(char name,int age)在C中会将方法名称编译成 _fun,而C++代码以C的方式进行编译会带上参数类型用来区分：_fun_char_int;
        此时，如果在C++中调用过一个C的方法void fun(char name,int age)，不填加extern "C",会报错。因为C++会去C中找_fun_char_int方法。
        而填加了extern "C"，就可以链接到了。因为会将方法名编译成_fun，这个方法可以在C中找到。
    4. JNIEXPORT：在Linux中定义为：__attribute__ ((visibility ("default"))) 。这个有点类似于Java中的public。保证生成的动态库可以被其他项目调用。
    5. JNICALL：在Windows中JNICALL 被定义为 __stdcall。__stdcall 用于 定义 函数入栈规则 ( 从右到左 ) , 和 堆栈清理规则 ;
                在Linux中 没有进行定义 , 直接置空 
    6. 在C中写JNI需要注意：不加extern "C",因为这个是C++中的语法。 C中的JNIEnv本身就是个一级指针。而方法中的JNIEnv* env 就成了一个二级指针。
```c
// 如何再C中写JNI
// C中写JNI
// C中的JNIEnv*env 是一个结构体指针
// 而 JNIEnv本身就是一个结构体指针：typedef const struct JNINativeInterface* JNIEnv;
// 即 JNIEnv* env是一个二级指针，需要先*env  获取他的一级指针，才能调用结构体指针JNIEnv的方法
// 而且C和C++的区别是。C++有对象的概念，C中没有。所以C中的方法比C++中的方法要多传一个env给函数
JNIEXPORT
jstring
JNICALL
Java_com_android_learn_1jni_Day1Activity_changeNameByC(JNIEnv *env, jclass jobject1){
    jstring  name = (*env)->NewStringUTF(env,"JTL is best");

    return name;
}
```
#### 2.JNI中的基本类型对应的C++中的类型
    // jboolean: unsigned char 1字节 8位
    // jbyte: signed char 1字节 8位
    // jchar: unsigned short 2字节 16位
    // jshort: short  2字节 16位
    // jint: int  4字节 32位
    // jfloat: float 4字节  32位
    // jlong: long  8字节 64位
    // jdouble: double 8字节 64位
    // jsize: int 4字节 32位
#### 3.JNI方法中的签名
    签名SIG：void(V),boolean(Z),byte(B),char(C),short(S),int(I),float(F),double(D),long(J),引用类型(L包名/类名);  
    举例：String：Ljava/lang/String;
    注意事项：引用类型要以;结尾。其他类型则不用
#### 4. JNI中的回收
    1. 回收局部变量：env->DeleteLocalRef()
    2. 回收全局变量：env->DeleteGlobalRef()
    3. 释放数组：env->Release***()  env->ReleaseIntArrayElements()

#### 5. JNI获取类的方法
    1. FindClass：通过全类名： jclass clazz = env->FindClass("com/android/learn_jni/Day2Activity$Student");
    2. GetObjectClass：通过对象：jclass clazz = env->GetObjectClass(thiz);

#### 6. JNI修改Java数据
    1. 基本数据类型：
        GetFieldID获取对象属性，
        GetDoubleField获取属性的值，
        SetDoubleField设置属性的值
    2. 数组：
        基本数据类型：GetIntArrayElements，直接返回数组指针
        引用数据类型：GetObjectArrayElement，获取具体位置的对象。
        刷新数据回Java：ReleaseIntArrayElements(,,JNI_OK);
        JNI_OK(0) :对JAVA数组进行更新，释放内存空间
        JNI_COMMIT(1) :对JAVA数组进行更新，不释放内存空间
        JNI_ABORT(2):不更新JAVA数组，释放内存空间

#### 7. JNI调用Java方法
    1. 获取class：  jclass clazz = env->GetObjectClass(thiz);
    2. 获取MethodId：jmethodID  jmethodId = env->GetMethodID(clazz,"add","(ILjava/lang/String;)I");
    3. 生成对应参数：jstring args = env->NewStringUTF("C++里传来的第一个参数是95");
    4. 调用JNI中对象的Call方法：jint data = env->CallIntMethod(thiz,jmethodId,95,args);

    注意事项：构造函数的方法名是  <init>  ，返回值  必须是V；
#### 8. JNI创建Java对象
    1. 获取class：jclass clazz_student = env->FindClass("com/android/learn_jni/Day2Activity$Student");
    2. 创建对象：   
        1.AllocObject:不执行构造函数
            1. env->AllocObject(clazz_student)
        2.NewObject：执行构造函数
            1. 获取构造器methodID:jmethodID jmethodId = env->GetMethodID(clazz_student,"<init>", "(Ljava/lang/String;I)V");
            2 创建对象：NewObject：jobject student_c = env->NewObject(clazz_student,jmethodId,29);//创建对象，需要手动回收
    3. 回收对象：env->DeleteLocalRef(student_c);
#### 9. JNI全局对象
    1. 无论是否在JNI方法中。所有对象都是局部对象。正规情况下都需要手动回收。方法弹栈时会自动回收。
    2. 局部变量升级为全局变量：env->NewGlobalRef(clazz_activity); // 将局部变量，改为全局变量。
    3. 全局变量需要手动回收：env->DeleteGlobalRef(clazz_activity)。
    4. 对象即便回收了，指针也不为null，记得手动置为null。


### 动态注册和静态注册
#### 注册种类
    ·静态注册：最常规的注册，Java_包名_类名_方法名(JNIEnv *env ,jobject thiz);
    ·动态注册：通过env->RegisterNatives();方法来注册，通常写在JNI_OnLoad方法里
#### 动态注册优点：
    ·在OnLoad时全部注册完毕，而不像静态注册，每次调用的时候才注册，节省性能
    ·更加安全，静态注册的话，可以通过Java中的方法名反推出JNI中的方法名称，安全性较差。
        动态注册的话，可以随意指定C++中的方法名称,反编译后安全性更高。

#### 动态注册
    ·JNINativeMethod:结构体。第一个参数是Java中的方法名，第二个：方法签名，第三个：C++中的结构体指针
    ·在JNI_OnLoad方法中进行注册
    ·env->RegisterNatives(要注册的方法所在的类, 要注册的JNINativeMethod, 注册个数);
    ·动态注册的方法，自动带JNIEnv和jobject。也可以省略不写
    ·动态注册的方法，可以通过签名来区别Java中重载的方法。
    ·不同的Android版本对应不同的JNI_VERSION,JNI_OnLoad时最好返回最高版本的JNI_VERSION，否则会报错。

```c++
// Bad JNI version returned from JNI_OnLoad
// android 1.6-4.3 version >= JNI_VERSION_1_2 
// android 4.4-7.1 version == JNI_VERSION_1_2 | JNI_VERSION_1_4 | JNI_VERSION_1_6
void dynamicRegister(){}

jstring dynamicRegister1(JNIEnv *env, jobject obj, jint age){}

void nativeThread1(JNIEnv *env, jobject obj){}

void nativeThread2(JNIEnv *env, jobject obj){}

static JNINativeMethod methods[] = {
        {"dynamicRegister",                  "()V",                   (void *) dynamicRegister},
        {"dynamicRegister",                  "(I)Ljava/lang/String;", (jstring *) dynamicRegister1},
        {"nativeThread_AttachCurrentThread", "()V",                   (void *) nativeThread1},
        {"nativeThread_GetEnv",              "()V",                   (void *) nativeThread2}
};
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
```
#### JNIEnv和jobject
    1. JNIEnv 和线程相对应，不能跨线程使用
    2. jobject 属于局部变量，也不能跨线程使用（每一个线程有一个栈，不同的线程对应不同的栈）
    3. jobject 属于局部变量，不能跨函数方法使用（每一个函数对应一个栈帧。栈帧中的局部变量不能跨栈帧使用）
    4. jobject 改为全局变量(env->NewGlobalRef(obj);)，就可以跨函数使用

#### C++创建子线程
    1. #include "pthread.h"
    2. 创建tid pthread_t tid = 199366（如果第三步的线程创建成功，这里的值会被修改为线程id）
    3. pthread_create(pthread_t,线程属性通常为null,函数名(线程运行的函数类似于Runnable),第三个参数里运行函数里的参数，无参为null)
    4. 线程创建成功返回0
    5.pthread_join(tid,nullptr); 当前线程等待tid线程执行完毕,在继续执行
```c++
    pthread_t tid = 199366;
    pthread_create(&tid, nullptr, cppRunThread2, nullptr);
```
#### JavaVM,JniEnv,jobject
    1. 一个进程对应一个JavaVM
    2. 获取JavaVM的方法有两种:
        ·JNI_OnLoad中保存一份
        ·env.GetJavaVM()
    3. JNIEnv和线程绑定，不同的线程，JNIEnv不是同一个示实例
    4. Java创建线程时，自动创建了JNIEnv，该JNIEnv可以通过应用类加载器加载自定义类.
    5. C++创建的线程，没有创建JNIEnv，需要手动关联
    6. Java创建的线程，可以通过vm->GetEnv()，来获取JNIEnv
    7. C++创建的线程，可以通过vm->AttachCurrentThread,来关联JNIEnv，此种方法的JNIEnv无法加载自定义类
    8. JNI静态方法（静态/动态注册）中的jclass，和调用类绑定，不同的类，jclass是不同的地址
    9. JNI普通方法（静态/动态注册）中的jobject和调用的对象实例绑定，不同的对象，此处的jobject是不同的。
    