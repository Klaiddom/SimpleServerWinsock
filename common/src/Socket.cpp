#include "../headers/Socket.h"


GeneralSocket::GeneralSocket(std::string &ip_addr, int port) {
    this->ip_addr = ip_addr;
    raw_socket = socket(AF_INET, SOCK_STREAM, 0);
    test_socket(raw_socket);
    socket_info.sin_family = AF_INET;
    socket_info.sin_port = htons(port);
    socket_info.sin_addr.s_addr = inet_addr(ip_addr.c_str());
}

void GeneralSocket::updateRaw(SOCKET new_socket) {
    test_socket(new_socket);
    raw_socket = new_socket;
}

void GeneralSocket::listen(int n_queues) {
    int listen_result = ::listen(raw_socket, n_queues);
    test_socket(listen_result);

    std::cout << "Started listening on address: " <<
              inet_ntoa(socket_info.sin_addr) << " port: " <<
              ntohs(socket_info.sin_port) << std::endl;
}

sockaddr* GeneralSocket::getAddress() {
    return (sockaddr*) &socket_info;
}

void GeneralSocket::test_socket(int subj) {
    if (subj < 0) {
        perror("Failed to test...");
        exit(EXIT_FAILURE);
    }
}

int GeneralSocket::getRaw(){
    return raw_socket;
}

sockaddr_in GeneralSocket::getSocketInfo() {
    return socket_info;
}

GeneralSocket::~GeneralSocket() {
    closesocket(raw_socket);
    std::cout << "Stopped socket on address: " << ip_addr << " port: " << socket_info.sin_port << std::endl;
}



ServerSocket::ServerSocket(std::string &ip_addr, int port) : GeneralSocket(ip_addr, port) {
    connect2network();
}

void ServerSocket::connect2network() {
    int socket_info_size = sizeof(socket_info);
    connection = ::bind(raw_socket, (sockaddr *) &socket_info, socket_info_size);
    test_socket(connection);
}


ClientSocket::ClientSocket(std::string &ip_addr, int port) : GeneralSocket(ip_addr, port){
}

void ClientSocket::connect2network() {
    int socket_info_size = sizeof(socket_info);
    connection = ::connect(raw_socket, (sockaddr *) &socket_info, socket_info_size);
    test_socket(connection);
}
