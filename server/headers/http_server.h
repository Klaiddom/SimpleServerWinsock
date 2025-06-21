#ifndef SANDBOX_HTTP_SERVER_H
#define SANDBOX_HTTP_SERVER_H

#include <iostream>
#include "../../common/headers/Socket.h"
#include "../../common/headers/message.h"

namespace http {

    class TCPServer {
    ServerSocket* serverSocket;
    static const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    WSAData  m_wsaData;
    int connectionsPerSocket;
    int connection_link;
    bool accepted = false;
    int timeout = 1000;
    std::string incoming_msg;
    bool decoding_msg = false;

    void startup(std::string& ip_addr, int port);

    int accept();
    void handle();
    void respond(int c);

    public:
        TCPServer(std::string& ip_addr, int port, int connectionsPerSocket);
        ~TCPServer();
        void run();
        void shutdown();

        void writeToConsole(std::string msg);
    };

}
#endif
