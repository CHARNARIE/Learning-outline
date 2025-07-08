#include "InetAddress.h"

InetAddress::InetAddress(const string &ip, unsigned short port) {
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = inet_addr(ip.c_str());
    _addr.sin_port = htons(port);
}

InetAddress::InetAddress(const struct sockaddr_in &addr) {
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = addr.sin_addr.s_addr;
    _addr.sin_port = addr.sin_port;
}

string InetAddress::getIp() {
    return string(inet_ntoa(_addr.sin_addr));
}

unsigned short InetAddress::getPort() {
    return _addr.sin_port;
}

struct sockaddr_in *InetAddress::getInetAddressPtr() {
    return &_addr;
}