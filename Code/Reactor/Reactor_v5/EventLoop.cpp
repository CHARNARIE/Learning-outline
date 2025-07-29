#include "EventLoop.h"
#include "Acceptor.h"
#include "TcpConnection.h"

EventLoop::EventLoop(Acceptor &acceptor, size_t maxEvents)
    : _epfd(createEpoll()), _isLooping(false), _acceptor(acceptor), _eventFd(createEventFd()) {
    if (maxEvents == 0) {
        throw "构造参数错误";
    }
    _epollEvents.reserve(maxEvents);
    addFd(_acceptor.fd());
    addFd(_eventFd);
}

EventLoop::~EventLoop() {
    close(_epfd);
    close(_eventFd);
}

// 持续wait()监听
void EventLoop::loop() {
    _isLooping = true;
    while (_isLooping) {
        wait();
    }
}

void EventLoop::unLoop() {
    _isLooping = false;
}

void EventLoop::setNewConnectionCallback(functionCallback &&func) {
    _newConnection = std::move(func);
}

void EventLoop::setMessageCallback(functionCallback &&func) {
    _message = std::move(func);
}

void EventLoop::setCloseCallback(functionCallback &&func) {
    _close = std::move(func);
}

int EventLoop::createEventFd() {
    int fd = eventfd(0, 0);
    if (fd < 0) {
        perror("createEventFd: ");
        return -1;
    } else {
        return fd;
    }
}

void EventLoop::handleRead() {
    uint64_t one = 1;
    ssize_t ret = read(_eventFd, &one, sizeof(uint64_t));
    if (ret != sizeof(uint64_t)) {
        perror("handleRead: ");
    }
}

void EventLoop::wakeup() {
    uint64_t one = 1;
    ssize_t ret = write(_eventFd, &one, sizeof(uint64_t));
    if (ret != sizeof(uint64_t)) {
        perror("wakeup: ");
    }
}

void EventLoop::doPendingTasks() {
    vector<Task> temp;
    std::unique_lock<mutex> ul{_mutex};
    temp.swap(_pendings);
    ul.unlock();

    for (auto &func : temp) {
        func();
    }
}

void EventLoop::runInLoop(Task &&task) {
    std::unique_lock<mutex> ul{_mutex};
    _pendings.push_back(std::move(task));
    ul.unlock();
    wakeup();
}

void EventLoop::handelNewConnection() {
    int connFd = _acceptor.accept();
    addFd(connFd);
    _conns[connFd] = std::make_shared<TcpConnection>(connFd, this);
    cout << _conns[connFd]->toString() << "建立连接" << endl;

    _conns[connFd]->setNewConnectionCallback(_newConnection);
    _conns[connFd]->setMessageCallback(_message);
    _conns[connFd]->setCloseCallback(_close);

    _conns[connFd]->newConnectionCallback();
}

void EventLoop::handelMessage(int fd) {
    if (_conns.count(fd)) {
        if (!_conns[fd]->isClosed()) {
            _conns[fd]->messageCallback();
        } else {
            cout << _conns[fd]->toString() << "断开连接" << endl;
            _conns[fd]->closeCallback();
            delFd(fd);
            _conns.erase(fd);
        }
    }
}

int EventLoop::createEpoll() {
    return epoll_create(1);
}

void EventLoop::wait() {
    int readySet = epoll_wait(_epfd, _epollEvents.data(), _epollEvents.capacity(), -1);
    if (readySet == -1 && errno == EINTR) {
        return;
    } else if (readySet == -1) {
        throw "epoll出错";
    } else {
        if ((size_t)readySet == _epollEvents.capacity()) {
            _epollEvents.reserve(2 * _epollEvents.capacity());
        }
        int listenFd = _acceptor.fd();
        for (int i = 0; i < readySet; ++i) {
            int fd = _epollEvents[i].data.fd;
            if (fd == listenFd) {
                handelNewConnection();
            } else if (fd == _eventFd) {
                handleRead();
                doPendingTasks();
            } else {
                handelMessage(fd);
            }
        }
    }
}

void EventLoop::addFd(int fd) {
    epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = fd;
    epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &event);
}

void EventLoop::delFd(int fd) {
    epoll_event event;
    event.data.fd = fd;
    epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, &event);
}