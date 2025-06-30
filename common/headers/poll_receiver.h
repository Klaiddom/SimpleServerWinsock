
#ifndef SANDBOX_POLL_RECIEVER_H
#define SANDBOX_POLL_RECIEVER_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <map>
#include <queue>
#include <thread>
#include "Socket.h"
#include "message.h"
#include "packet.h"

namespace http{
    class POLLReceiver{

        static const int max_clients = 100;
        WSAPOLLFD poll_group[max_clients];
        int connected_users = 0;
        int wait_millisecond = 10;
        static const int buffer_size = 1024;
        char input_buffer[buffer_size];
        std::map<SOCKET, int> socket_map;
        std::map<SOCKET, std::queue<Message* > > msgs;
        std::map<SOCKET, std::vector<Packet* > > packets;
        std::thread* process_thread;

        void extractMessages(SOCKET socket, int received_bytes);
        std::vector<Packet* >* splitIntoPackets(int received_bytes);
        Message* composeMessage(SOCKET socket);


    public:
        POLLReceiver();
        void addConnection(GeneralSocket* listening_socket);
        void process();
        void start();
        std::queue<Message*>* getMessages(GeneralSocket* socket);
        void removeConnection(SOCKET raw_socket);
        ~POLLReceiver();
    };
}

#endif
