#ifndef _TASK_QUEUE_H
#define _TASK_QUEUE_H

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>

using std::condition_variable;
using std::mutex;
using std::queue;
using std::unique_lock;

using Task = std::function<void()>;

class TaskQueue {
public:
    TaskQueue(size_t capacity);

    bool isFull();

    bool isEmpty();

    void push(Task &&task);

    Task pop();

    void wakeAll();

private:
    size_t _capacity;
    queue<Task> _queue;
    mutex _mutex;
    condition_variable _notEmpty;
    condition_variable _notFull;
    bool _flag = true;
};

#endif