
#include "../headers/http_server.h"

namespace http{

    TCPServer::TCPServer(std::string& ip_addr, int port){
        startup(ip_addr, port);
        accept_thread = new std::thread(&TCPServer::accept, this);
        receiver = new POLLReceiver();
    };

    TCPServer::~TCPServer(){
        shutdown();
    };

    void TCPServer::startup(std::string& ip_addr, int port) {
        if(WSAStartup(MAKEWORD(2, 0), &m_wsaData) != 0){
            std::cerr << "WSAStartup failed" << std::endl;
        }
        serverSocket = new ServerSocket(ip_addr, port);
        serverSocket->listen(size_of_queue_in);
    }

    void TCPServer::shutdown() {
        delete serverSocket;
        WSACleanup();
    }

    void TCPServer::writeToConsole(std::string msg) {
        std::cout << msg << std::endl;
    }

    void TCPServer::run(){
        receiver->start();
        accept_thread->detach();
        while(true){
            for(auto& [user_id, user]: users){
                auto socket = user->getUserData()->socket;
                auto received_msgs = receiver->getMessages(socket);
                if(!received_msgs->empty()){
                    auto last_msg = received_msgs->front();
                    outputMessage(last_msg);
                    if(last_msg->getTo() == "all"){
                        for(auto& [another_user_id, another_user]: users){
                            if(user_id != another_user_id)
                                sender.send(*last_msg, another_user->getUserData()->socket);
                        }
                    } else if(users.find(last_msg->getTo()) != users.end() && last_msg->getTo() != user_id){
                        sender.send(*last_msg, users[last_msg->getTo()]->getUserData()->socket);
                    } else{
                        std::cout << "Unknown user " << last_msg->getTo() << ", can't send a message" << std::endl;
                    }
                    received_msgs->pop();
                    delete(last_msg);
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    void TCPServer::accept() {
        int addr_len = sizeof(serverSocket->getSocketInfo());
        std::string dummy_address = "192.168.1.1";
        auto q = new std::queue<Message* >();
        while(true) {
            auto new_connection_link = ::accept(serverSocket->getRaw(), serverSocket->getAddress(), &addr_len);
            auto new_client_link = new ClientSocket(dummy_address, 8080);
            new_client_link->updateRaw(new_connection_link);
            receiver->addConnection(new_client_link);

            while(q->size() == 0){
                q = receiver->getMessages(new_client_link);
            }

            auto last_msg = q->front();
            auto new_user = new User(new_client_link);
            new_user->setId(last_msg->getFrom());
            users[new_user->getId()] = new_user;

            std::cout << "Connection is accepted and running on << " << new_connection_link << std::endl;
        }
    }

    void TCPServer::outputMessage(Message* msg) {
        if(msg->getContent()->find("client_id:") != std::string::npos)
            return;
        std::string consoleOutput;
        consoleOutput += msg->getFrom();
        consoleOutput += "->";
        consoleOutput += msg->getTo();
        consoleOutput += ": ";
        consoleOutput += *msg->getContent();
        writeToConsole(consoleOutput);
    }
}