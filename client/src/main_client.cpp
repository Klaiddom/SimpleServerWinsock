#include <iostream>
#include "../headers/http_client.h"

int main(){
    std::string ip_addr = "192.168.31.49";
    std::string send_info, response;

    sockaddr_in server_info;
    server_info.sin_family = AF_INET;
    server_info.sin_addr.s_addr = inet_addr(ip_addr.c_str());
    server_info.sin_port = htons(8001);

    std::cout << "Enter your nickname: ";
    std::getline(std::cin, send_info);
    http::TCPClient client(send_info, ip_addr, 8002);
    client.setServerAddress((sockaddr *) &server_info, sizeof(server_info));
    std::cout << "Server address is set" << std::endl;
    client.connect();
    client.start_listen();

    while(true){
        send_info.clear();
        std::getline(std::cin, send_info);
        if(send_info == "exit()"){
            client.disconnect();
            break;
        }
        client.send(send_info);
    }
    return EXIT_SUCCESS;
}
