#ifndef _TCPCONNECTION_H
#define _TCPCONNECTION_H

#include "EventLoop.h"
#include "InetAddress.h"
#include "Socket.h"
#include "SocketIO.h"
#include <cstring>
#include <functional>
#include <memory>
#include <sstream>
#include <string>

using std::function;
using std::ostringstream;
using std::shared_ptr;
using std::string;

class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
public:
    using functionCallback = std::function<void(const shared_ptr<TcpConnection> &)>;

    explicit TcpConnection(int fd);

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

private:
    SocketIO _sockIO;
    Socket _sock;
    InetAddress _localAddr;
    InetAddress _peerAddr;

    functionCallback _newConnection;
    functionCallback _message;
    functionCallback _close;

    InetAddress getLocalAddr();
    InetAddress getPeerAddr();
};

#endif //_TCPCONNECTION_H