#include "Acceptor.h"

Acceptor::Acceptor(const string &ip, unsigned short port)
    : _addr(ip, port) {}

void Acceptor::ready() {
    setReuseAddr();
    setReusePort();
    bind();
    listen();
}

int Acceptor::accept() {
    int ret = ::accept(_sock.getFd(), nullptr, nullptr);
    return ret;
}

int Acceptor::fd() {
    return _sock.getFd();
}

void Acceptor::setReuseAddr() {
    int opt = 1;
    setsockopt(_sock.getFd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

void Acceptor::setReusePort() {
    int opt = 1;
    setsockopt(_sock.getFd(), SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
}

void Acceptor::bind() {
    ::bind(_sock.getFd(), (struct sockaddr *)_addr.getInetAddressPtr(), sizeof(*_addr.getInetAddressPtr()));
}

void Acceptor::listen() {
    ::listen(_sock.getFd(), 10);
    cout << "等待客户端连接..." << endl;
}