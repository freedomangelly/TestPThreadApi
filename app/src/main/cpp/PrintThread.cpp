//
// Created by freed on 2020/2/2.
//

#include <pthread.h>
#include <unistd.h>
#include "PrintThread.h"
#include "android_log.h"

PrintThread::PrintThread(char *test) {
    LOGI("create  PrintThread %s",test);
    this->test=test;
}

void PrintThread::print() {
    LOGI("pring  PrintThread %s",test);
}

void* thread(void *arg){
    LOGI("this is a thread and arg = %d",(int*)arg);
    return 0;
}

void PrintThread::runThread() {
    pthread_t th;
    int ret;
    int arg=10;
    ret=pthread_create(&th,NULL,thread,&arg);
    if(ret != 0){
        LOGI("Create thread error.\n");
        return;
    }
    void *thread_ret;
    LOGI("This is the main process.\n");
//    pthread_join(th, &thread_ret);
    pthread_detach(th);
    LOGI("thread_ret = %d.\n",(int)thread_ret);
}

void* xunhuanxiancheng(void *arg){
    char* c=(char*)arg;
    LOGI("参数%s \n",c);
    int i=0;
    for (;i<10;i++){
        LOGI("循环%d\n",i);
        if(i==5){
            pthread_exit(reinterpret_cast<void *>(1090000000));
        }
    }
    return reinterpret_cast<void *>(100000222);
}

void PrintThread::runThread2() {
    pthread_t tid;
    pthread_create(&tid, NULL, xunhuanxiancheng, (void *) "线程！！！！");

    void *status;
    pthread_join(tid,&status);
    LOGI("返回%d\n",(int)status);
}


pthread_mutex_t mutex;
int i=0;
void* thr_fun(void* arg){
    pthread_mutex_lock(&mutex);
    char* no= static_cast<char *>(arg);
    for(;i<5;i++){
        LOGI("%s thread, i:%d\n",no,i);
        sleep(1);
    }
    i=0;
    pthread_mutex_unlock(&mutex);
    return 0;
}

void PrintThread::mutexThread() {
    pthread_t  tid1,tid2;
    pthread_mutex_init(&mutex,NULL);

    pthread_create(&tid1, nullptr, thr_fun, (void *) "No1");
    pthread_create(&tid2, nullptr, thr_fun, (void *) "No2");

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_mutex_destroy(&mutex);
}

//消费者数量
#define CONSUMER_NUM 2
//生产者数量
#define PRODUCER_NUM 1

pthread_t  pids[CONSUMER_NUM+PRODUCER_NUM];
//产品数量
int ready=0;
//互斥锁
pthread_mutex_t mutex2;
//条件变量
pthread_cond_t has_prodect;
//生产方法
void* producer(void* arg){
    int no= reinterpret_cast<int>(arg);
    for (;;){
        pthread_mutex_lock(&mutex2);
        ready++;
        LOGI("producer %d, produce product\n",ready);
        //通知消费者，有新的产品可以消费了
        //会阻塞输出
        pthread_cond_signal(&has_prodect);
        LOGI("producer %d, singal\n",no);
        pthread_mutex_unlock(&mutex2);
        sleep(1);
    }
    return 0;
}
//消费者
void* consumer(void* arg) {
    int num = (int) arg;
    for (;;) {
        pthread_mutex_lock(&mutex2);
        //while?
        //superious wake ‘惊群效应’
        while (ready == 0) {
            //没有产品，继续等待
            //1.阻塞等待has_product被唤醒
            //2.释放互斥锁，pthread_mutex_unlock
            //3.被唤醒时，解除阻塞，重新申请获得互斥锁pthread_mutex_lock
            LOGI("%d consumer wait\n", num);
            pthread_cond_wait(&has_prodect, &mutex2);
        }
        //有产品，消费产品
        ready--;
        LOGI("%d consume product\n", ready);
        pthread_mutex_unlock(&mutex2);
        sleep(1);
    }
    return 0;
}


    void PrintThread::mutexThread2() {
        //初始化互斥锁和条件变量
        pthread_mutex_init(&mutex2,NULL);
        pthread_cond_init(&has_prodect,NULL);
        LOGI("init\n");

        int i;
        for(i=0; i<PRODUCER_NUM;i++){
            //生产者线程
            LOGI("%d\n",i);
            pthread_create(&pids[i],NULL,producer,(void*)i);
        }

        for(i=0; i<CONSUMER_NUM;i++){
            //消费者线程
            pthread_create(&pids[PRODUCER_NUM+i],NULL,consumer,(void*)i);
        }

        //等待
        sleep(10);
        for(i=0; i<PRODUCER_NUM+CONSUMER_NUM;i++){
            pthread_join(pids[i],NULL);
        }

        //销毁互斥锁和条件变量
        pthread_mutex_destroy(&mutex2);
        pthread_cond_destroy(&has_prodect);
    }






