
#include "../headers/http_server.h"

namespace http{

    TCPServer::TCPServer(std::string& ip_addr, int port, int connectionsPerSocket){
        this->connectionsPerSocket = connectionsPerSocket;
        startup(ip_addr, port);

    };

    TCPServer::~TCPServer(){
        shutdown();
    };

    void TCPServer::startup(std::string& ip_addr, int port) {
        if(WSAStartup(MAKEWORD(2, 0), &m_wsaData) != 0){
            std::cerr << "WSAStartup failed" << std::endl;
        }
        serverSocket = new ServerSocket(ip_addr, port);
        serverSocket->listen(connectionsPerSocket);
    }

    void TCPServer::shutdown() {
        delete serverSocket;
        WSACleanup();
    }

    void TCPServer::writeToConsole(std::string msg) {
        std::cout << msg << std::endl;
    }

    void TCPServer::run(){
        int counter = 0;
        while(true){
            if(!accepted)
                accept();

            handle();
            respond(std::max(counter, 1));
        }
    }

    int TCPServer::accept() {
        int addr_len = sizeof(serverSocket->getSocketInfo());
        connection_link = ::accept(serverSocket->getRaw(), serverSocket->getAddress(), &addr_len);
        std::cout << "Connection is accepted" << std::endl;
//        setsockopt(connection_link, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

        accepted = true;
        return 0;
    }

    void TCPServer::handle() {
        std::cout << "W8 for data" << std::endl;
        memset(buffer, 0, BUFFER_SIZE * sizeof(char));
        int bytes_received = recv(connection_link, buffer, BUFFER_SIZE, 0);
        writeToConsole(buffer);
    }

    void TCPServer::respond(int c) {
        std::string body = "Hello" + std::to_string(c);
        std::string response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: " + std::to_string(body.size()) + "\r\n"
                                                                    "\r\n" +
                body;
        int bytes_sent = ::send(connection_link, response.c_str(), response.size(), 0);
        writeToConsole("Response sent");
//        ::closesocket(connection_link);
//        writeToConsole("Closed connection");
    }

}