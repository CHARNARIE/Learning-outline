#include "Acceptor.h"
#include "EventLoop.h"

using functionCallback = std::function<void(const std::shared_ptr<TcpConnection> &)>;

void newConnection(const shared_ptr<TcpConnection> &func) {
    cout << "新连接到来时, main定义的函数回调" << endl;
}

void message(const shared_ptr<TcpConnection> &func) {
    string str = func->receive();
    if (str.size() != 0) {
        cout << "收到：" << str << endl;
    }
}

void closeConnection(const shared_ptr<TcpConnection> &func) {
    cout << "回调函数：对方关闭连接" << endl;
}

int main() {
    Acceptor acceptor{"127.0.0.1", 12345};
    acceptor.ready();
    EventLoop eventLoop{acceptor, 1024};
    eventLoop.setNewConnectionCallback(functionCallback(newConnection));
    eventLoop.setMessageCallback(functionCallback(message));
    eventLoop.setCloseCallback(functionCallback(closeConnection));
    eventLoop.loop();
    return 0;
}