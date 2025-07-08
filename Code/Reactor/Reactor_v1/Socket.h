#ifndef _SOCKET_H
#define _SOCKET_H

#include <arpa/inet.h>
#include <unistd.h>

class Socket {
public:
    Socket();

    explicit Socket(int fd);

    ~Socket();

    int getFd();

private:
    int _fd;

    Socket(const Socket &) = delete;

    Socket &operator=(const Socket &) = delete;
};

#endif //_SOCKET_H