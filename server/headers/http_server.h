#ifndef SANDBOX_HTTP_SERVER_H
#define SANDBOX_HTTP_SERVER_H

#include <iostream>
#include <map>
#include <thread>
#include "../../common/headers/Socket.h"
#include "../../common/headers/message.h"
#include "../../common/headers/poll_receiver.h"
#include "../../common/headers/user.h"

namespace http {

    class TCPServer {
        ServerSocket* serverSocket;
        WSAData  m_wsaData;
        const int size_of_queue_in = 10;
        std::map<std::string, User* > users;
        std::thread* accept_thread;
        POLLReceiver* receiver;
        MessageSender sender;


        void startup(std::string& ip_addr, int port);

        void accept();
        void outputMessage(Message* msg);

        public:
            TCPServer(std::string& ip_addr, int port);
            ~TCPServer();
            void run();
            void shutdown();

            void writeToConsole(std::string msg);
    };

}
#endif
