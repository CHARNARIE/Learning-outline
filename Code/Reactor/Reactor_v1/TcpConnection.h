#ifndef _TCPCONNECTION_H
#define _TCPCONNECTION_H

#include "SocketIO.h"
#include <cstring>
#include <string>

using std::string;

class TcpConnection {
public:
    explicit TcpConnection(int fd);

    string receive();

    void send(const string &msg);

private:
    SocketIO _sockIO;
};

#endif //_TCPCONNECTION_H