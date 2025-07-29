#include "TcpConnection.h"
#include "TcpServer.h"
#include "ThreadPool.h"
#include <functional>
#include <iostream>
#include <memory>
#include <string>

using std::cout;
using std::endl;
using std::shared_ptr;
using std::string;

using functionCallback = std::function<void(const std::shared_ptr<TcpConnection> &)>;

ThreadPool *gPool = nullptr;

void newConnection(const shared_ptr<TcpConnection> &func) {
    cout << "新连接到来时, main定义的函数回调" << endl;
}

void message(const shared_ptr<TcpConnection> &func) {
    string str = func->receive();
    if (str.size() != 0) {
        cout << "收到：" << str << endl;
    }
    // 这里收到信息，创建任务将其加入任务队列异步执行
    // 执行完毕后会自动调用sendInLoop创建新的返回任务
    // 异步回复给客户端
    MyTask task{str, func};
    gPool.addTask(std::bind(&MyTask::process, task));
}

void closeConnection(const shared_ptr<TcpConnection> &func) {
    cout << "回调函数：对方关闭连接" << endl;
}

class MyTask {
public:
    MyTask(const string &msg, const shared_ptr<TcpConnection> &conn)
        : _msg(msg), _conn(conn) {}

    // 处理数据
    void process() {
        // 在这里处理数据
        // 处理(_msg);
        // 处理完毕
        _conn->sendInLoop(_msg);
    }

private:
    string _msg;
    shared_ptr<TcpConnection> _conn;
};

int main() {
    ThreadPool pool{3, 10};
    pool.start();
    gPool = &pool;

    TcpServer svr{"127.0.0.1", 12345, 1024};
    svr.setAllCallback(newConnection, message, closeConnection);
    svr.start();
    svr.stop();

    return 0;
}