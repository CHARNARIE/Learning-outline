#include "EventLoop.h"
#include "TcpConnection.h"

EventLoop::EventLoop(Acceptor &acceptor, size_t maxEvents)
    : _epfd(createEpoll()), _isLooping(false), _acceptor(acceptor) {
    if (maxEvents == 0) {
        throw "构造参数错误";
    }
    _epollEvents.reserve(maxEvents);
    addFd(_acceptor.fd());
}

EventLoop::~EventLoop() {
    close(_epfd);
}

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

void EventLoop::handelNewConnection() {
    int connFd = _acceptor.accept();
    addFd(connFd);
    _conns[connFd] = std::make_shared<TcpConnection>(connFd);
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