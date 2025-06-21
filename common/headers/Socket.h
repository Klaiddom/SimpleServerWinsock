#ifndef SANDBOX_SOCKET_H
#define SANDBOX_SOCKET_H
#include <string>
#include <winsock.h>
#include <iostream>

class GeneralSocket {
protected:
    std::string ip_addr;
    SOCKET raw_socket;
    sockaddr_in socket_info;
    int connection;

    virtual void connect2network() = 0;
    void test_socket(int subj);

public:
    GeneralSocket(std::string& ip_addr, int port);
    ~GeneralSocket();
    void listen(int n_queues);
    int getRaw();
    sockaddr* getAddress();
    void updateRaw(SOCKET new_socket);
    sockaddr_in getSocketInfo();
};


class ServerSocket : public GeneralSocket{
protected:
    void connect2network();

public:
    ServerSocket(std::string& ip_addr, int port);

};

class ClientSocket : public GeneralSocket{
protected:
    void connect2network();

public:
    ClientSocket(std::string& ip_addr, int port);
};

#endif
