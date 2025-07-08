#include "TcpServer.h"

TcpServer::TcpServer(const string &ip, unsigned short port, size_t maxEvents)
    : _acceptor(ip, port), _eventLoop(_acceptor, maxEvents) {}

void TcpServer::start() {
    _acceptor.ready();
    _eventLoop.loop();
}

void TcpServer::stop() {
    _eventLoop.unLoop();
}

void TcpServer::setAllCallback(functionCallback &&newConn, functionCallback &&msg, functionCallback &&close) {
    _eventLoop.setNewConnectionCallback(std::move(newConn));
    _eventLoop.setMessageCallback(std::move(msg));
    _eventLoop.setCloseCallback(std::move(close));
}