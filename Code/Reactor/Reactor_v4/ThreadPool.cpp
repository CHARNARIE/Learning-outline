#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t threadNum, size_t queueSize)
    : _threadNum(threadNum),
      _taskQueue(queueSize),
      _queueSize(queueSize) {}

void ThreadPool::start() {
    for (int i = 0; i < _threadNum; ++i) {
        _threads.push_back(thread{&ThreadPool::doTask, this});
    }
}

void ThreadPool::stop() {
    while (!_taskQueue.isEmpty()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    _isAlive = false;
    _taskQueue.wakeAll();
    for (auto &th : _threads) {
        th.join();
    }
}

void ThreadPool::addTask(Task &&task) {
    if (task) {
        _taskQueue.push(std::move(task));
    }
}

Task ThreadPool::getTask() {
    return _taskQueue.pop();
}

void ThreadPool::doTask() {
    while (_isAlive) {
        Task task = getTask();
        if (task && _isAlive) {
            task();
        }
    }
}