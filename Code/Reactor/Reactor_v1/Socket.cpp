#include "Socket.h"

Socket::Socket()
    : _fd(socket(AF_INET, SOCK_STREAM, 0)) {
}

Socket::~Socket() {
    close(_fd);
}

int Socket::getFd() {
    return _fd;
}