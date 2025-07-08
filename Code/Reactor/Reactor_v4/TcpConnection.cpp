#include "TcpConnection.h"
#include "EventLoop.h"

TcpConnection::TcpConnection(int fd, EventLoop *eventLoop)
    : _sockIO(fd), _sock(fd), _localAddr(getLocalAddr()), _peerAddr(getPeerAddr()), _loop(eventLoop) {}

string TcpConnection::receive() {
    string str;
    char buf[256];
    while (true) {
        memset(buf, 0, 256);
        int readSize = _sockIO.readLine(buf, 256);
        if (readSize > 0) {
            str += buf;
        } else if (readSize < 0) {
            throw "读取出错";
        }
        if (readSize < 256) {
            break;
        }
    }
    return str;
}

void TcpConnection::send(const string &msg) {
    _sockIO.writen(msg.c_str(), msg.size());
}

void TcpConnection::setNewConnectionCallback(const functionCallback &func) {
    _newConnection = func;
}

void TcpConnection::setMessageCallback(const functionCallback &func) {
    _message = func;
}

void TcpConnection::setCloseCallback(const functionCallback &func) {
    _close = func;
}

void TcpConnection::newConnectionCallback() {
    if (_newConnection) {
        _newConnection(shared_from_this());
    }
}

void TcpConnection::messageCallback() {
    if (_message) {
        _message(shared_from_this());
    }
}

void TcpConnection::closeCallback() {
    if (_close) {
        _close(shared_from_this());
    }
}

string TcpConnection::toString() {
    ostringstream oss;
    oss << "服务端" << _localAddr.getIp() << ":" << _localAddr.getPort()
        << "---客户端" << _peerAddr.getIp() << ":" << _peerAddr.getPort();
    return oss.str();
}

InetAddress TcpConnection::getLocalAddr() {
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    getsockname(_sock.getFd(), (struct sockaddr *)&addr, &len);
    return InetAddress{addr};
}

InetAddress TcpConnection::getPeerAddr() {
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    getpeername(_sock.getFd(), (struct sockaddr *)&addr, &len);
    return InetAddress{addr};
}

bool TcpConnection::isClosed() {
    int fd = _sock.getFd();
    char buf[1];
    int ret = recv(fd, buf, sizeof(buf), MSG_PEEK);
    if (ret == 0) {
        return true; // 对端关闭
    } else if (ret < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
        return true; // 其他错误视为断开
    }
    return false;
}

void TcpConnection::sendInLoop(const string &msg) {
    if (_loop) {
        _loop->runInLoop(std::bind(&TcpConnection::send, this, msg));
    }
}
