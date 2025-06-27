
#include "../headers/http_client.h"

using namespace http;

TCPClient::TCPClient(std::string& ip_addr, int port){
    if(WSAStartup(MAKEWORD(2, 0), &m_wsaData) != 0){
        std::cerr << "WSAStartup failed" << std::endl;
    }
    std::cout << "Creating client socket" << std::endl;
    socket = new ClientSocket(ip_addr, port);
}

void TCPClient::setServerAddress(sockaddr* server2connect, int size){
    server_info = server2connect;
    server_info_size = size;
}

void TCPClient::connect(){
    if(server_info == nullptr)
        std::cerr << "NO server to connect to" << std::endl;
    std::cout << "Trying to connect" << std::endl;
    auto server_socket = ::connect(socket->getRaw(), server_info, server_info_size);
    std::cout << "Server socket: " << server_socket << std::endl;
    if(server_socket < 0)
        std::cout << "Failed to connect to server" << std::endl;
    user = new User(socket);
    user->setId("Client" + std::to_string(socket->getRaw()));
    std::string service_info = "client_id: " + *user->getId();
    send(service_info);
    listen_thread = new std::thread(&TCPClient::listen, this);
    listen_thread->detach();
}

void TCPClient::send(std::string& msg){
    http::Message msg_obj(msg, *user->getId());
    user->sendResponse(msg_obj);
}

void TCPClient::disconnect(){
    ::closesocket(socket->getRaw());
}

TCPClient::~TCPClient(){
    delete socket;
    WSACleanup();
}

void TCPClient::listen(){
    while(true){
        auto msg = user->getLastMsg();
        if(msg)
            std::cout << *msg->getContent() << std::endl;
    }
}

