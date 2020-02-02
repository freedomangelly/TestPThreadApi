#include <jni.h>
#include <string>
#include "stdio.h"
#include "pthread.h"
#include "android_log.h"
#include "PrintThread.h"




extern "C" JNIEXPORT void JNICALL
Java_com_liuy_testthread_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
//    runThread();
    char* hel="helloo";
    PrintThread *thread1;
    thread1=new PrintThread(hel);
//    thread1->print();
    thread1->runThread();
//    thread1->runThread2();
//thread1->mutexThread2();
}
