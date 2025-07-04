#ifndef SANDBOX_HTTP_CLIENT_H
#define SANDBOX_HTTP_CLIENT_H

#include "../../common/headers/Socket.h"
#include "../../common/headers/message.h"
#include "../../common/headers/sender.h"
#include "../../common/headers/user.h"
#include <string>
#include <thread>
#include <chrono>

namespace http {
    class TCPClient{
        MessageSender sender;
        MessageReceiver receiver;
        const int modulo = 10000;
        sockaddr* server_info = nullptr;
        int server_info_size = 0;
        WSAData m_wsaData;
        User* user;
        std::thread* listen_thread;
        bool is_connected = false;

        void creationProtocol(std::string& ip_addr, int port);


    public:
        TCPClient(std::string& ip_addr, int port);
        TCPClient(std::string& login, std::string& ip_addr, int port);
        ~TCPClient();
        void setServerAddress(sockaddr* server2connect, int size);
        void connect();
        void send(std::string& msg);
        void send(std::string& msg, std::string& to_user_id);
        void start_listen();
        void disconnect();
        void listen();
    };
}

#endif
