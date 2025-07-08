#ifndef _EVENT_LOOP_H
#define _EVENT_LOOP_H

#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <vector>

using std::function;
using std::map;
using std::mutex;
using std::shared_ptr;
using std::vector;

class Acceptor;
class TcpConnection;

using functionCallback = std::function<void(const shared_ptr<TcpConnection> &)>;
using Task = std::function<void()>;

class EventLoop {
public:
    EventLoop(Acceptor &acceptor, size_t maxEvents);

    ~EventLoop();

    void loop();

    void unLoop();

    void setNewConnectionCallback(functionCallback &&func);

    void setMessageCallback(functionCallback &&func);

    void setCloseCallback(functionCallback &&func);

    // 创建用于通知的文件描述符
    int createEventFd();

    // 封装read
    void handleRead();

    // 封装write
    void wakeup();

    // 执行任务，将数据发给客户端
    void doPendingTasks();

    // 存放任务到vector中，并且唤醒Reactor/EventLoop
    void runInLoop(Task &&task);

private:
    int _epfd;
    bool _isLooping;
    mutex _mutex;
    Acceptor &_acceptor;
    int _eventFd;
    vector<struct epoll_event> _epollEvents;
    vector<Task> _pendings;
    map<int, shared_ptr<TcpConnection>> _conns;
    functionCallback _newConnection;
    functionCallback _message;
    functionCallback _close;

    int createEpoll();

    void wait();

    void addFd(int fd);

    void delFd(int fd);

    void handelNewConnection();

    void handelMessage(int fd);
};

#endif