#include "Acceptor.h"
#include "EventLoop.h"

class TcpServer {
public:
    TcpServer(const string &ip, unsigned short port, size_t maxEvents);

    void start();

    void stop();

    void setAllCallback(functionCallback &&newConn, functionCallback &&msg, functionCallback &&close);

private:
    Acceptor _acceptor;
    EventLoop _eventLoop;
};