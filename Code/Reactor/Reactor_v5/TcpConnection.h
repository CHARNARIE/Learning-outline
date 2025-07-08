#ifndef _TCPCONNECTION_H
#define _TCPCONNECTION_H

#include "InetAddress.h"
#include "Socket.h"
#include "SocketIO.h"
#include <cstring>
#include <functional>
#include <memory>
#include <sstream>
#include <string>

using std::ostringstream;
using std::shared_ptr;
using std::string;

class EventLoop;

class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
public:
    using functionCallback = std::function<void(const shared_ptr<TcpConnection> &)>;

    explicit TcpConnection(int fd, EventLoop *eventLoop);

    string receive();

    void send(const string &msg);

    string toString();

    void setNewConnectionCallback(const functionCallback &func);

    void setMessageCallback(const functionCallback &func);

    void setCloseCallback(const functionCallback &func);

    void newConnectionCallback();

    void messageCallback();

    void closeCallback();

    bool isClosed();

    // 线程池使用TcpConnection的对象发送数据给EventLoop
    void sendInLoop(const string &msg);

private:
    SocketIO _sockIO;
    Socket _sock;
    InetAddress _localAddr;
    InetAddress _peerAddr;
    EventLoop *_loop;

    functionCallback _newConnection;
    functionCallback _message;
    functionCallback _close;

    InetAddress getLocalAddr();
    InetAddress getPeerAddr();
};

#endif //_TCPCONNECTION_H