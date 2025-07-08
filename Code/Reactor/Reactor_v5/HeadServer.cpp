#include "HeadServer.h"
#include "TcpConnection.h"

MyTask::MyTask(const string &msg, const shared_ptr<TcpConnection> &conn)
    : _msg(msg), _conn(conn) {}

// 处理数据
void MyTask::process() {
    // 在这里处理数据
    // 处理(_msg);
    // 处理完毕
    _conn->sendInLoop(_msg);
}

HeadServer::HeadServer(size_t threadNum, size_t queueSize, const string &ip, unsigned short port, size_t maxEvents)
    : _pool(threadNum, queueSize), _tcpSvr(ip, port, maxEvents) {}

void HeadServer::start() {
    _pool.start();
    using namespace std::placeholders;
    _tcpSvr.setAllCallback(std::bind(&HeadServer::newConnection, this, _1),
                           std::bind(&HeadServer::message, this, _1),
                           std::bind(&HeadServer::closeConnection, this, _1));
    _tcpSvr.start();
}

void HeadServer::stop() {
    _pool.stop();
    _tcpSvr.stop();
}

void HeadServer::newConnection(const shared_ptr<TcpConnection> &conn) {
    cout << "新连接到来时, main定义的函数回调" << endl;
}

void HeadServer::message(const shared_ptr<TcpConnection> &conn) {
    string str = conn->receive();
    if (str.size() != 0) {
        cout << "收到：" << str << endl;
        conn->send("已收到");
    }
}

void HeadServer::closeConnection(const shared_ptr<TcpConnection> &conn) {
    cout << "回调函数：对方关闭连接" << endl;
}