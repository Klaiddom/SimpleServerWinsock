
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
    server_socket = ::connect(socket->getRaw(), server_info, server_info_size);
    std::cout << "Server socket: " << server_socket << std::endl;
    if(server_socket < 0)
        std::cout << "Failed to connect to server" << std::endl;
}

void TCPClient::send(std::string& msg){

    http::Message msg_obj(msg);
    std::cout << "Message total size: " << msg_obj.size() << std::endl;
    auto chunks = msg_obj.getChunks();
    for(auto& chunk: *chunks){
        int bytes_sent = ::send(socket->getRaw(), chunk.c_str(), chunk.size(), 0);
        if(bytes_sent > 0)
            std::cout << "Sent:: " << bytes_sent << " bytes!" << std::endl;
        else
            std::cout << WSAGetLastError() << std::endl;
    }
//    ::shutdown(server_socket, 1);
}

void TCPClient::disconnect(){
    ::closesocket(server_socket);
}

TCPClient::~TCPClient(){
    delete socket;
    WSACleanup();
}

