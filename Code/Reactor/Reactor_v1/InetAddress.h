#ifndef _INETADDRESS_H
#define _INETADDRESS_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>

using std::string;
class InetAddress {
public:
    InetAddress(const string &ip, unsigned short port);

    InetAddress(const struct sockaddr_in &addr);

    string getIp();

    unsigned short getPort();

    struct sockaddr_in *getInetAddressPtr();

private:
    struct sockaddr_in _addr;
};

#endif //_INETADDRESS_H