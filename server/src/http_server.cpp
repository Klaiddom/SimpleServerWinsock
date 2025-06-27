
#include "../headers/http_server.h"

namespace http{

    TCPServer::TCPServer(std::string& ip_addr, int port, int connectionsPerSocket){
        this->connectionsPerSocket = connectionsPerSocket;
        startup(ip_addr, port);
        accept_thread = new std::thread(&TCPServer::accept, this);
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
        accept_thread->detach();
        while(true){
            if(!received_msgs.empty()){
                auto last_msg = received_msgs.front();
//                std::cout << "Message is processed" << std::endl;
                for(auto user: users){
                    if(*user->getId() != last_msg->getFrom()){
                        user->sendResponse(*last_msg);
//                        std::cout << "Respond is sent" << std::endl;
                    }
                }
                received_msgs.pop();
                delete(last_msg);
            } else{
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
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
            respond(user);
        }
    }

    void TCPServer::handle(User* user) {
        auto msg = user->getLastMsg();
        if(!msg)
            return;
        if(user->getId()){
            std::string consoleOutput;
            consoleOutput += *user->getId();
            consoleOutput += ": ";
            consoleOutput += *msg->getContent();
            writeToConsole(consoleOutput);

            msg->setFrom(*user->getId());
            received_msgs.push(msg);
        } else if(msg->getContent()->find("client_id: ") != std::string::npos){
            user->setId(msg->getContent()->substr(11, msg->getContent()->size() - 12));
        }
    }

    void TCPServer::respond(http::User* user) {
    }

}