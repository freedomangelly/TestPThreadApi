//
// Created by freed on 2020/2/2.
//

#ifndef TESTTHREAD_PRINTTHREAD_H
#define TESTTHREAD_PRINTTHREAD_H


class PrintThread {
public :
    PrintThread(char *test);
    char *test;

public:
    void print();
    void runThread();
    void runThread2();
    void mutexThread();
    void mutexThread2();
};


#endif //TESTTHREAD_PRINTTHREAD_H
