#ifndef _SOCKETIO_H
#define _SOCKETIO_H

#include <iostream>
#include <unistd.h>

class SocketIO {
public:
    explicit SocketIO(int fd);

    ~SocketIO();

    int readn(char *buf, int len);

    int writen(const char *buf, int len);

    int readLine(char *buf, int len);

private:
    int _fd;

    SocketIO(const SocketIO &) = delete;

    SocketIO &operator=(const SocketIO &) = delete;
};

#endif //_SOCKETIO_H