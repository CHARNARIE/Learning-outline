#include "TaskQueue.h"

TaskQueue::TaskQueue(size_t capacity) : _capacity(capacity) {}

bool TaskQueue::isFull() {
    return _queue.size() >= _capacity;
}

bool TaskQueue::isEmpty() {
    return _queue.empty();
}

void TaskQueue::push(Task &&task) {
    unique_lock<mutex> ul{_mutex};
    while (isFull()) {
        _notFull.wait(ul);
    }
    _queue.push(std::move(task));
    _notEmpty.notify_one();
}

Task TaskQueue::pop() {
    unique_lock<mutex> ul{_mutex};
    while (isEmpty() && _flag) {
        _notEmpty.wait(ul);
    }
    if (_flag && !_queue.empty()) {
        Task temp = std::move(_queue.front());
        _queue.pop();
        _notFull.notify_one();
        return temp;
    }
    return Task{};
}

void TaskQueue::wakeAll() {
    _flag = false;
    _notEmpty.notify_all();
}
