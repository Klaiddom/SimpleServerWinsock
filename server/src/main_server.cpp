#include "../headers/main_server.h"

int main(){
    std::string ip_addr = "192.168.31.49";

    http::TCPServer server = http::TCPServer(ip_addr, 8001, 2);
    server.run();
    server.shutdown();

    return 0;
}
