#ifndef SANDBOX_HTTP_SERVER_H
#define SANDBOX_HTTP_SERVER_H

#include <iostream>
#include <thread>
#include "../../common/headers/Socket.h"
#include "../../common/headers/message.h"
#include "../../common/headers/receiver.h"
#include "../../common/headers/user.h"

namespace http {

    class TCPServer {
    ServerSocket* serverSocket;
    static const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    WSAData  m_wsaData;
    int connectionsPerSocket;
    bool accepted = false;
    int timeout = 1000;
    std::string incoming_msg;
    bool decoding_msg = false;
    MessageReceiver receiver;
    std::vector<User* > users;
    std::vector<std::thread* > active_user_threads;
    std::thread* accept_thread;


    void startup(std::string& ip_addr, int port);

    void accept();
    void handle(User* user);
    void respond(int c);
    void process_user(User* user);

    public:
        TCPServer(std::string& ip_addr, int port, int connectionsPerSocket);
        ~TCPServer();
        void run();
        void shutdown();

        void writeToConsole(std::string msg);
    };

}
#endif
