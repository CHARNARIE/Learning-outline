#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H

#include "TaskQueue.h"
#include <iostream>
#include <thread>
#include <vector>

using std::thread;
using std::vector;

class ThreadPool {
public:
    ThreadPool(size_t threadNum, size_t queueSize);

    void start();

    void stop();

    void addTask(Task &&task);

    Task getTask();

    void doTask();

private:
    TaskQueue _taskQueue;
    size_t _threadNum;
    vector<thread> _threads;
    size_t _queueSize;
    bool _isAlive = true;
};

#endif