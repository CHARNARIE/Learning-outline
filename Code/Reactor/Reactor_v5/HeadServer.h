#ifndef _HEAD_SERVER_H
#define _HEAD_SERVER_H

#include "TcpServer.h"
#include "ThreadPool.h"
#include <memory>
#include <string>

using std::shared_ptr;
using std::string;

class TcpConnection;

class MyTask {
public:
    MyTask(const string &msg, const shared_ptr<TcpConnection> &conn);

    // 处理数据
    void process();

private:
    string _msg;
    shared_ptr<TcpConnection> _conn;
};

class HeadServer {
public:
    HeadServer(size_t threadNum, size_t queueSize, const string &ip, unsigned short port, size_t maxEvents);

    // 服务器的启动与停止
    void start();

    void stop();

    // 三个回调
    void newConnection(const shared_ptr<TcpConnection> &conn);

    void message(const shared_ptr<TcpConnection> &conn);

    void closeConnection(const shared_ptr<TcpConnection> &conn);

private:
    ThreadPool _pool;  // 线程池子对象
    TcpServer _tcpSvr; // TcpServer子对象
};

#endif