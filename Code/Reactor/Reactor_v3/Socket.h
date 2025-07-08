#ifndef _SOCKET_H
#define _SOCKET_H

#include <sys/socket.h>
#include <unistd.h>

class Socket {
public:
    Socket();

    ~Socket();

    explicit Socket(int fd);

    int getFd();

private:
    int _fd;

    Socket(const Socket &) = delete;

    Socket &operator=(const Socket &) = delete;
};

#endif //_SOCKET_H