
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
        memset(buffer, 0, BUFFER_SIZE * sizeof(char));
        int bytes_received = recv(connection_link, buffer, BUFFER_SIZE, 0);
        if(bytes_received > 0){
            std::string tmp;
            writeToConsole(buffer);
            tmp = buffer;
            if(!decoding_msg && tmp.find(http::Message::general_header) != std::string::npos) {
                decoding_msg = true;
            } else if(tmp.rfind(http::Message::tail) != std::string::npos){
                decoding_msg = false;
                writeToConsole("Full msg received =:");
                writeToConsole(incoming_msg);
                incoming_msg.clear();
            } else if(decoding_msg && tmp.find(http::Message::safeguard_begin) != std::string::npos &&
                      tmp.rfind(http::Message::safeguard_end) != std::string::npos){
                incoming_msg += tmp.substr(http::Message::safeguard_begin.size(),
                                           tmp.size() - http::Message::safeguard_begin.size() - http::Message::safeguard_end.size());
            }
        }
    }

    void TCPServer::respond(int c) {
//        std::string body = "Hello" + std::to_string(c);
//        std::string response =
//                "HTTP/1.1 200 OK\r\n"
//                "Content-Type: text/plain\r\n"
//                "Content-Length: " + std::to_string(body.size()) + "\r\n"
//                                                                    "\r\n" +
//                body;
//        int bytes_sent = ::send(connection_link, response.c_str(), response.size(), 0);
//        writeToConsole("Response sent");
//        ::closesocket(connection_link);
//        writeToConsole("Closed connection");
    }

}