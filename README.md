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
        