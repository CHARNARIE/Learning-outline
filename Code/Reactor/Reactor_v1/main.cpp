#include "Acceptor.h"

int main() {
    Acceptor acceptor{"127.0.0.1", 12345};
    acceptor.ready();
    TcpConnection tcpConnection{acceptor.accept()};
    while (true) {
        string str = tcpConnection.receive();
        for (auto &c : str) {
            if (isalpha(c)) {
                c = toupper(c);
            }
        }
        tcpConnection.send(str);
    }
    return 0;
}