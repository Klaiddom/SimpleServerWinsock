#ifndef SANDBOX_HTTP_CLIENT_H
#define SANDBOX_HTTP_CLIENT_H

#include "../../common/headers/Socket.h"
#include "../../common/headers/message.h"
#include <string>

namespace http {
    class TCPClient{
        ClientSocket* socket;
        sockaddr* server_info = nullptr;
        int server_info_size = 0;
        WSAData  m_wsaData;
        SOCKET server_socket;

    public:
        TCPClient(std::string& ip_addr, int port);
        ~TCPClient();
        void setServerAddress(sockaddr* server2connect, int size);
        void connect();
        void send(std::string& msg);
        void disconnect();
    };
}

#endif //SANDBOX_HTTP_CLIENT_H
