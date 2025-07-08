#ifndef _ACCEPTOR_H
#define _ACCEPTOR_H

#include "InetAddress.h"
#include "Socket.h"
#include "SocketIO.h"
#include "TcpConnection.h"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

using std::cout;
using std::endl;
using std::string;

class Acceptor {
public:
    Acceptor(const string &ip, unsigned short port);

    void ready();

    int accept();

private:
    Socket _sock;
    InetAddress _addr;

    void setReuseAddr();

    void setReusePort();

    void bind();

    void listen();
};

#endif //_ACCEPTOR_H