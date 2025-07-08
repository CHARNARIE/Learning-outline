#ifndef _TCP_SERVER_H
#define _TCP_SERVER_H

#include "Acceptor.h"
#include "EventLoop.h"
#include <string>

using std::string;

class TcpServer {
public:
    TcpServer(const string &ip, unsigned short port, size_t maxEvents);

    void start();

    void stop();

    void setAllCallback(functionCallback &&newConn, functionCallback &&msg, functionCallback &&close);

private:
    Acceptor _acceptor;
    EventLoop _eventLoop;
};

#endif