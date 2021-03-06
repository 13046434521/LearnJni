//
// Created by jiatianlong on 2022/5/7.
//

#ifndef LEARNJNI_LOG_H
#define LEARNJNI_LOG_H
#include <android/log.h>
#define TAG "LearnJniCpp"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__);
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG,__VA_ARGS__);
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__);
#endif //LEARNJNI_LOG_H
