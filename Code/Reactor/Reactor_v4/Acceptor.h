#ifndef _ACCEPTOR_H
#define _ACCEPTOR_H

#include "InetAddress.h"
#include "Socket.h"
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

class Acceptor {
public:
    Acceptor(const string &ip, unsigned short port);

    void ready();

    int accept();

    int fd();

private:
    Socket _sock;
    InetAddress _addr;

    void setReuseAddr();

    void setReusePort();

    void bind();

    void listen();
};

#endif //_ACCEPTOR_H