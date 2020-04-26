#include "MainServer.h"

int main()
{
    MainServer server("192.168.0.103",8888);
    server.start();
    return 0;
}

