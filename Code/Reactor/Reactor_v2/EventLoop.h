#ifndef _EVENT_LOOP_H
#define _EVENT_LOOP_H

#include "Acceptor.h"
#include "InetAddress.h"
#include "Socket.h"
#include "SocketIO.h"
#include "TcpConnection.h"
#include <functional>
#include <map>
#include <memory>
#include <sys/epoll.h>
#include <vector>

class TcpConnection;

using std::map;
using std::shared_ptr;
using std::vector;

using functionCallback = std::function<void(const shared_ptr<TcpConnection> &)>;

class EventLoop {
public:
    EventLoop(Acceptor &acceptor, size_t maxEvents);

    ~EventLoop();

    void loop();

    void unLoop();

    void setNewConnectionCallback(functionCallback &&func);

    void setMessageCallback(functionCallback &&func);

    void setCloseCallback(functionCallback &&func);

private:
    int _epfd;
    vector<struct epoll_event> _epollEvents;
    bool _isLooping;
    Acceptor &_acceptor;
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