#include "SocketIO.h"

SocketIO::SocketIO(int fd) : _fd(fd) {}

SocketIO::~SocketIO() {
    close(_fd);
}

int SocketIO::readn(char *buf, int len) {
    int left = len;  // 剩余要读取的字节数
    char *ptr = buf; // 当前写入位置
    int readSize = 0;
    while (left > 0) {
        readSize = read(_fd, ptr, left);
        if (readSize < 0) {
            if (errno == EINTR) {
                readSize = 0;
            } else {
                return -1;
            }
        } else if (readSize == 0) {
            break;
        }
        left -= readSize;
        ptr += readSize;
    }
    return len - left;
}

int SocketIO::writen(const char *buf, int len) {
    int left = len;
    const char *ptr = buf;
    int wrote = 0;
    while (left > 0) {
        wrote = write(_fd, ptr, left);
        if (wrote < 0) {
            if (errno == EINTR) {
                wrote = 0;
            } else {
                return -1;
            }
        } else if (wrote == 0) {
            break;
        }
        left -= wrote;
        ptr += wrote;
    }
    return len - left;
}

int SocketIO::readLine(char *buf, int len) {
    int count = 0;
    char c;
    while (count < len - 1) {
        int ret = read(_fd, &c, 1);
        if (ret < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                return -1;
            }
        } else if (ret == 0) {
            break;
        }
        buf[count++] = c;
        if (c == '\n') {
            break;
        }
    }
    buf[count] = '\0';
    return count;
}