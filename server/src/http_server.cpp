
#include "../headers/http_server.h"

namespace http{

    TCPServer::TCPServer(std::string& ip_addr, int port, int connectionsPerSocket){
        this->connectionsPerSocket = connectionsPerSocket;
        startup(ip_addr, port);
        accept_thread = new std::thread(&TCPServer::accept, this);
        accept_thread->join();
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
//        int counter = 0;
//        while(true){
//            if(!accepted)
//                accept();
//            else{
//                handle();
//                respond(std::max(counter, 1));
//            }
//        }
    }

    void TCPServer::accept() {
        int addr_len = sizeof(serverSocket->getSocketInfo());
        std::string dummy_address = "192.168.1.1";
        while(true) {
            auto new_connection_link = ::accept(serverSocket->getRaw(), serverSocket->getAddress(), &addr_len);
            auto new_client_link = new ClientSocket(dummy_address, 8080);
            new_client_link->updateRaw(new_connection_link);
            auto new_user = new User(new_client_link);
            users.push_back(new_user);
            std::thread new_user_thread(&TCPServer::process_user, this, new_user);
            active_user_threads.push_back(&new_user_thread);
            new_user_thread.detach();
            std::cout << "Connection is accepted and running on << " << new_connection_link << std::endl;
        }
    }

    void TCPServer::process_user(http::User *user){
        while(true){
            handle(user);
        }
    }

    void TCPServer::handle(User* user) {
//        auto msg = receiver.retrieveLastMessage(user->getSocket()->getRaw());
        user->receiveMsg();
        auto msg = user->getLastMsg();

        if(!msg)
            return;

        if(user->getId()){
            std::string consoleOutput;
            consoleOutput += *user->getId();
            consoleOutput += ": ";
            consoleOutput += *msg->getContent();
            writeToConsole(consoleOutput);
        } else if(msg->getContent()->find("client_id: ") != std::string::npos){
            user->setId(msg->getContent()->substr(11, msg->getContent()->size() - 12));
        }
        delete msg;
//        memset(buffer, 0, BUFFER_SIZE * sizeof(char));
//        int bytes_received = recv(connection_link, buffer, BUFFER_SIZE, 0);
//        if(bytes_received > 0){
//            std::string tmp;
//            writeToConsole(buffer);
//            tmp = buffer;
//            if(!decoding_msg && tmp.find(http::Message::general_header) != std::string::npos) {
//                decoding_msg = true;
//            } else if(tmp.rfind(http::Message::tail) != std::string::npos){
//                decoding_msg = false;
//                writeToConsole("Full msg received =:");
//                writeToConsole(incoming_msg);
//                incoming_msg.clear();
//            } else if(decoding_msg && tmp.find(http::Message::safeguard_begin) != std::string::npos &&
//                      tmp.rfind(http::Message::safeguard_end) != std::string::npos){
//                incoming_msg += tmp.substr(http::Message::safeguard_begin.size(),
//                                           tmp.size() - http::Message::safeguard_begin.size() -
//                                           http::Message::safeguard_end.size());
//            }
//        }
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