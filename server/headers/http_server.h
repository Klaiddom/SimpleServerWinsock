#ifndef SANDBOX_HTTP_SERVER_H
#define SANDBOX_HTTP_SERVER_H

#include <iostream>
#include <map>
#include <thread>
#include "../../common/headers/Socket.h"
#include "../../common/headers/message.h"
#include "../../common/headers/receiver.h"
#include "../../common/headers/user.h"

namespace http {

    class TCPServer {
        ServerSocket* serverSocket;
        WSAData  m_wsaData;
        int connectionsPerSocket;
        std::vector<User* > users;
        std::vector<std::thread* > active_user_threads;
        std::thread* accept_thread;
        std::queue<Message*> received_msgs;


        void startup(std::string& ip_addr, int port);

        void accept();
        void handle(User* user);
        void respond(User* user);
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
