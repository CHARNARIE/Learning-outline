#include "HeadServer.h"

int main() {
    HeadServer svr{3, 10, "127.0.0.1", 12345, 1024};
    svr.start();
    svr.stop();
    return 0;
}