#include "../headers/http_client.h"

using namespace http;

TCPClient::TCPClient(std::string& ip_addr, int port){
    creationProtocol(ip_addr, port);
    auto now = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch() );
    std::string user_id = "Client" + std::to_string(now.count() % modulo);
    user->setId(user_id);
}
TCPClient::TCPClient(std::string &login, std::string &ip_addr, int port) {
    creationProtocol(ip_addr, port);
    user->setId(login);
}

void TCPClient::creationProtocol(std::string &ip_addr, int port) {
    if(WSAStartup(MAKEWORD(2, 0), &m_wsaData) != 0){
        std::cerr << "WSAStartup failed" << std::endl;
    }
    std::cout << "Creating client socket" << std::endl;
    auto socket = new ClientSocket(ip_addr, port);
    user = new User(socket);
}

void TCPClient::setServerAddress(sockaddr* server2connect, int size){
    server_info = server2connect;
    server_info_size = size;
}

void TCPClient::connect(){
    if(server_info == nullptr)
        std::cerr << "NO server to connect to" << std::endl;
    std::cout << "Trying to connect" << std::endl;
    SOCKET socket = user->getUserData()->socket->getRaw();
    auto server_connection_result = ::connect(socket, server_info, server_info_size);
    if(server_connection_result < 0)
        std::cout << "Failed to connect to server" << std::endl;
    else
        std::cout << "Successfully connected to server" << std::endl;
    std::string service_info = "client_id: " + user->getId();
    std::string to = user->getId();
    send(service_info, to);
    is_connected = true;
}

void TCPClient::send(std::string& msg){
    http::Message msg_obj(msg);
    std::string from = user->getId();
    msg_obj.setFrom(from);
    std::string to = "all";
    msg_obj.setTo(to);
    sender.send(msg_obj, user->getUserData()->socket);
}

void TCPClient::send(std::string &msg, std::string &to_user_id) {
    http::Message msg_obj(msg);
    std::string from = user->getId();
    msg_obj.setFrom(from);
    msg_obj.setTo(to_user_id);
    sender.send(msg_obj, user->getUserData()->socket);
}

TCPClient::~TCPClient(){
    disconnect();
    if(listen_thread->joinable())
        listen_thread->join();
    delete listen_thread;
    delete user;
    WSACleanup();
}

void TCPClient::start_listen() {
    listen_thread = new std::thread(&TCPClient::listen, this);
    listen_thread->detach();
}

void TCPClient::disconnect() {
    std::string disconnect_str = http::Safeguards::DISCONNECT;
    send(disconnect_str);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    is_connected = false;
}

void TCPClient::listen(){
    SOCKET connected_socket = user->getUserData()->socket->getRaw();
    while(is_connected){
        auto msg = receiver.retrieveLastMessage(connected_socket);
        if(msg)
            std::cout << msg->getFrom() << ": " << *msg->getContent() << std::endl;
    }
}

