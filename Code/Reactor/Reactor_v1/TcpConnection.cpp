#include "TcpConnection.h"

TcpConnection::TcpConnection(int fd) : _sockIO(fd) {}

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